
/* ===== FUNCTION _init @ 00012458 ===== */

int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  __gmon_start__();
  (*(code *)0x2be4)();
  iVar1 = __do_global_ctors_aux();
  return iVar1;
}



/* ===== FUNCTION main @ 000124e0 ===== */

undefined4 processEntry main(void)

{
  long lVar1;
  long lVar2;
  int iVar3;
  undefined4 uVar4;
  int in_a0;
  undefined4 *in_a1;
  
  if (in_a0 == 3) {
    lVar1 = strtol((char *)in_a1[1],(char **)0x0,10);
    lVar2 = strtol((char *)in_a1[2],(char **)0x0,10);
    printf("gpio number: %d, val: %d\n",lVar1,lVar2);
    gpio_init();
    iVar3 = gpio_configure(lVar1,1);
    if (iVar3 != 0) {
      puts("set gpio output fail.");
    }
    gpio_set_output(lVar1,lVar2 != 0);
    gpio_deinit();
    uVar4 = 0;
  }
  else {
    printf("Usage: %s <gpio number> <output val>\n",*in_a1);
    uVar4 = 0xffffffff;
  }
  return uVar4;
}



/* ===== FUNCTION deregister_tm_clones @ 00012a70 ===== */

/* WARNING: Removing unreachable block (ram,0x00012a9c) */
/* WARNING: Removing unreachable block (ram,0x00012aa4) */

void deregister_tm_clones(void)

{
  return;
}



/* ===== FUNCTION register_tm_clones @ 00012ab4 ===== */

/* WARNING: Removing unreachable block (ram,0x00012ae8) */
/* WARNING: Removing unreachable block (ram,0x00012af0) */

void register_tm_clones(void)

{
  return;
}



/* ===== FUNCTION __do_global_dtors_aux @ 00012b00 ===== */

/* WARNING: Removing unreachable block (ram,0x00012b84) */
/* WARNING: Removing unreachable block (ram,0x00012bb0) */

void __do_global_dtors_aux(void)

{
  if (completed_6286 == '\0') {
    __cxa_finalize(__dso_handle);
    deregister_tm_clones();
    completed_6286 = '\x01';
  }
  return;
}



/* ===== FUNCTION frame_dummy @ 00012be4 ===== */

void frame_dummy(void)

{
  if (__JCR_END__ != 0) {
    _Jv_RegisterClasses();
  }
  register_tm_clones();
  return;
}



/* ===== FUNCTION get_volume @ 00012c40 ===== */

uint get_volume(void)

{
  int __fd;
  uint uVar1;
  byte local_10 [8];
  
  local_10[0] = 0;
  __fd = open64("/dev/sndC0i2so",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    ioctl(__fd,0x4001080c,local_10);
    close(__fd);
    uVar1 = (uint)local_10[0];
  }
  return uVar1;
}



/* ===== FUNCTION api_cube_update_flip_screens @ 00012cc4 ===== */

void api_cube_update_flip_screens(int param_1)

{
  undefined4 uVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_18;
  undefined1 local_16;
  undefined1 local_15 [9];
  
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_16 = 0;
  local_15[0] = 0;
  get_screen_rotation_from_dtb(&local_18,local_15,&local_16);
  video_driver_mutex_lock();
  uVar1 = fbdev_get_color_format_src();
  video_driver_get_screen_size(&local_1c,&local_20);
  fbdev_deinit();
  uVar4 = local_16;
  uVar3 = local_15[0];
  uVar2 = local_18;
  if (param_1 == 0) {
    uVar4 = 0;
    uVar3 = 0;
    uVar2 = 0;
  }
  fbdev_setfilp(uVar2,uVar3,uVar4);
  fbdev_init(local_1c,local_20,uVar1);
  video_driver_update_screen();
  video_driver_mutex_unlock();
  return;
}



/* ===== FUNCTION set_backlight_value @ 00012dd8 ===== */

undefined4 set_backlight_value(undefined4 param_1)

{
  int __fd;
  undefined4 local_18 [3];
  
  __fd = open64("/dev/backlight",2);
  if (__fd < 0) {
    param_1 = 0xffffffff;
  }
  else {
    local_18[0] = param_1;
    write(__fd,local_18,4);
    fsync(__fd);
    close(__fd);
  }
  return param_1;
}



/* ===== FUNCTION cube_shmget_init @ 00012e74 ===== */

void * cube_shmget_init(char *param_1,size_t param_2)

{
  int iVar1;
  key_t __key;
  stat64 sStack_b8;
  void *local_18;
  
  iVar1 = __xstat64(3,param_1,&sStack_b8);
  if ((iVar1 == 0) && ((sStack_b8.st_uid & 0xf000) == 0x4000)) {
    __key = ftok(param_1,0x61);
    if (__key == -1) {
      perror("ftok");
      local_18 = (void *)0x0;
    }
    else {
      iVar1 = shmget(__key,param_2,0x3b6);
      if (iVar1 == -1) {
        perror("shmget");
        local_18 = (void *)0x0;
      }
      else {
        local_18 = shmat(iVar1,(void *)0x0,0);
        if (local_18 == (void *)0x0) {
          perror("shmat");
        }
      }
    }
  }
  else {
    local_18 = (void *)0x0;
  }
  return local_18;
}



/* ===== FUNCTION cube_shmget_game_status.part.1 @ 00012f80 ===== */

undefined4 cube_shmget_game_status_part_1(void)

{
  game_status_6355 = (undefined4 *)cube_shmget_init("/tmp/cubegame_status",4);
  if (game_status_6355 != (undefined4 *)0x0) {
    return *game_status_6355;
  }
  return 0;
}



/* ===== FUNCTION cube_memory_save.constprop.3 @ 00012fe0 ===== */

undefined4 cube_memory_save_constprop_3(undefined4 param_1)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  
  __fd = open64("/dev/persistentmem",2);
  if (__fd < 0) {
    uVar2 = 0xffffffff;
  }
  else {
    iVar1 = ioctl(__fd,0x800c2603,param_1);
    if (iVar1 < 0) {
      close(__fd);
      uVar2 = 0xffffffff;
    }
    else {
      close(__fd);
      uVar2 = 0;
    }
  }
  return uVar2;
}



/* ===== FUNCTION cube_memory_get.constprop.4 @ 00013084 ===== */

undefined4 cube_memory_get_constprop_4(undefined4 param_1)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  
  __fd = open64("/dev/persistentmem",0x4012);
  if (__fd < 0) {
    uVar2 = 0xffffffff;
  }
  else {
    iVar1 = ioctl(__fd,0x400c2602,param_1);
    if (iVar1 < 0) {
      close(__fd);
      uVar2 = 0xffffffff;
    }
    else {
      close(__fd);
      uVar2 = 0;
    }
  }
  return uVar2;
}



/* ===== FUNCTION api_cube_send_msg_to_process @ 00013128 ===== */

/* WARNING: Removing unreachable block (ram,0x00018038) */

undefined4 api_cube_send_msg_to_process(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uStack_a0;
  undefined1 auStack_9c [132];
  
  if (cube_pcmsg_id == -1) {
    cube_pcmsg_id = api_message_create_path("/tmp/process_msg",100,8);
    if (cube_pcmsg_id == -1) {
      return 0xffffffff;
    }
  }
  iVar1 = cube_pcmsg_id;
  uStack_a0 = 1;
  __memcpy_chk(auStack_9c,param_1,8,0x80);
  iVar1 = msgsnd(iVar1,&uStack_a0,8,0);
  if (iVar1 == -1) {
    perror("msgsnd failed");
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION api_cube_send_msg @ 000131c4 ===== */

/* WARNING: Removing unreachable block (ram,0x00018038) */

undefined4 api_cube_send_msg(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uStack_a0;
  undefined1 auStack_9c [132];
  
  if (cube_msg_id == -1) {
    cube_msg_id = api_message_create_path("/tmp/cube_process_msg",100,8);
    if (cube_msg_id == -1) {
      return 0xffffffff;
    }
  }
  iVar1 = cube_msg_id;
  uStack_a0 = 1;
  __memcpy_chk(auStack_9c,param_1,8,0x80);
  iVar1 = msgsnd(iVar1,&uStack_a0,8,0);
  if (iVar1 == -1) {
    perror("msgsnd failed");
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION api_cube_receive_msg @ 00013260 ===== */

/* WARNING: Removing unreachable block (ram,0x000181cc) */
/* WARNING: Removing unreachable block (ram,0x000181f4) */
/* WARNING: Removing unreachable block (ram,0x000181bc) */
/* WARNING: Removing unreachable block (ram,0x00018214) */
/* WARNING: Removing unreachable block (ram,0x0001819c) */
/* WARNING: Removing unreachable block (ram,0x000181b4) */
/* WARNING: Removing unreachable block (ram,0x0001821c) */

int api_cube_receive_msg(void)

{
  int iVar1;
  
  if (cube_msg_id == -1) {
    cube_msg_id = api_message_create_path("/tmp/cube_process_msg",100,8);
    if (cube_msg_id == -1) {
      return -1;
    }
  }
  iVar1 = api_message_receive_block();
  return -(uint)(iVar1 != 0);
}



/* ===== FUNCTION fbdev_aspect_ratio_wrok @ 00013300 ===== */

undefined4 fbdev_aspect_ratio_wrok(void)

{
  int __fd;
  int __fd_00;
  int iVar1;
  undefined4 uVar2;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined2 local_14;
  undefined2 local_12;
  
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_12 = 0;
  video_driver_mutex_lock();
  __fd = open64("/dev/dis",2);
  if (__fd < 0) {
    uVar2 = 0xffffffff;
    puts("open /dev/dis failed");
  }
  else {
    __fd_00 = open64("/dev/fb0",2);
    if (__fd_00 < 1) {
      uVar2 = 0xffffffff;
      puts("open /dev/fb0 failed");
      close(__fd);
    }
    else {
      local_30 = 1;
      iVar1 = ioctl(__fd,0xc00c0e0c,&local_30);
      if (iVar1 == 0) {
        close(__fd);
        local_20 = local_28;
        video_driver_get_screen_size(&local_18,&local_1c);
        local_24 = CONCAT22((short)local_1c,(short)local_18);
        ioctl(__fd_00,0x80040d05,&local_14);
        ioctl(__fd_00,0x80080d00,&local_24);
        close(__fd_00);
        uVar2 = 0;
      }
      else {
        uVar2 = 0xffffffff;
        close(__fd);
        close(__fd_00);
        puts("DIS_GET_SCREEN_INFO failed");
      }
    }
  }
  video_driver_mutex_unlock();
  return uVar2;
}



/* ===== FUNCTION api_get_hdmi_tx_connection_status @ 000134f4 ===== */

bool api_get_hdmi_tx_connection_status(int param_1)

{
  uint uVar1;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  char *local_18;
  char local_14 [4];
  int local_10;
  
  local_14[0] = '\0';
  if (game_status_6355 == (uint *)0x0) {
    local_10 = param_1;
    uVar1 = cube_shmget_game_status_part_1();
    param_1 = local_10;
  }
  else {
    uVar1 = *game_status_6355;
  }
  if ((uVar1 & 1) != 0) {
    if (param_1 != 0) {
      local_20 = 3;
      local_1e = 0xb1;
      local_1c = 1;
      local_18 = local_14;
      cube_memory_get_constprop_4(&local_20);
      return local_14[0] != '\0';
    }
    return true;
  }
  return false;
}



/* ===== FUNCTION cube_msg_pthread_cb @ 000135a8 ===== */

void cube_msg_pthread_cb(void)

{
  int iVar1;
  int local_60;
  undefined2 local_5c;
  int *local_58;
  uint local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  undefined1 local_38 [8];
  undefined *local_30;
  code *local_2c;
  
  local_30 = (undefined *)0x20000;
  local_2c = (code *)0x2c40;
  local_54 = 0;
  local_50 = 0;
LAB_00013630:
  do {
    while( true ) {
      do {
        iVar1 = api_cube_receive_msg(&local_54);
      } while (iVar1 != 0);
      if (local_54 != 0x102) break;
LAB_000136f0:
      api_cube_send_msg_to_process(&local_54);
    }
    if (local_54 < 0x103) {
      if (local_54 == 2) {
        local_60 = 3;
        local_3c = local_50;
        local_5c = 1;
        local_58 = &local_3c;
        cube_memory_save_constprop_3(&local_60);
      }
      else if (local_54 < 3) {
        if (local_54 == 0) {
          local_44 = 0;
          local_60 = 0x1e0001;
          local_5c = 1;
          local_58 = &local_44;
          iVar1 = cube_memory_get_constprop_4(&local_60);
          if (iVar1 == 0) {
            iVar1 = open64(local_30 + 0x1000,2);
            local_60 = -1;
            if (-1 < iVar1) {
              read(iVar1,&local_60,4);
              fsync(iVar1);
              close(iVar1);
            }
            if (local_44 != local_60) {
              local_4c = local_60;
              local_58 = &local_4c;
              local_60 = 0x1e0001;
              local_5c = 1;
              cube_memory_save_constprop_3(&local_60);
            }
          }
          local_60 = 3;
          local_5c = 1;
          local_58 = &local_44;
          iVar1 = cube_memory_get_constprop_4(&local_60);
          if ((iVar1 == 0) && (iVar1 = (*local_2c)(), iVar1 != local_44)) {
            local_58 = &local_48;
            local_60 = 3;
            local_5c = 1;
            local_48 = iVar1;
            cube_memory_save_constprop_3(&local_60);
          }
        }
        else if (local_54 == 1) {
          local_5c = 1;
          local_58 = &local_40;
          local_60 = 0x1e0001;
          local_40 = local_50;
          cube_memory_save_constprop_3(&local_60);
        }
      }
      else if ((local_54 == 0x100) || (local_54 == 0x101)) goto LAB_000136f0;
      goto LAB_00013630;
    }
    if (local_54 == 0x107) {
      api_cube_update_flip_screens(1);
    }
    else if (local_54 < 0x108) {
      if (local_54 == 0x103) goto LAB_000136f0;
      if (local_54 == 0x106) {
        api_cube_update_flip_screens(0);
      }
    }
    else if (local_54 == 0x108) {
      local_38[0] = (undefined1)local_50;
      local_60 = 0xb10003;
      local_5c = 1;
      local_58 = (int *)local_38;
      cube_memory_save_constprop_3(&local_60);
      iVar1 = api_get_hdmi_tx_connection_status(0);
      if (iVar1 != 0) {
        fbdev_aspect_ratio_wrok(local_50);
      }
    }
    else if (local_54 == 0xffff) {
                    /* WARNING: Subroutine does not return */
      pthread_exit((void *)0x0);
    }
  } while( true );
}



/* ===== FUNCTION cube_msg_pthread_init @ 000138fc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 cube_msg_pthread_init(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_28;
  undefined4 local_24;
  undefined1 *local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined1 local_14 [4];
  
  uVar1 = 0;
  if (cube_thread == 0) {
    local_18 = 0;
    local_14[0] = 0xff;
    local_24 = CONCAT22(local_24._2_2_,1);
    local_20 = local_14;
    local_1c = 0x104;
    local_28 = 0x1e0001;
    cube_memory_get_constprop_4(&local_28);
    iVar2 = api_get_hdmi_tx_connection_status(0);
    if (iVar2 == 0) {
      set_backlight_value(local_14[0]);
    }
    local_28 = 0;
    local_24 = 0;
    local_20 = (undefined1 *)0x0;
    iVar2 = open64("/dev/dis",1);
    if (-1 < iVar2) {
      local_20 = (undefined1 *)((uint)local_20 & 0xffffff00);
      local_28 = 1;
      local_24 = 1;
      ioctl(iVar2,0x800c0e04,&local_28);
      close(iVar2);
    }
    api_cube_send_msg_to_process(&local_1c);
    iVar2 = pthread_create((pthread_t *)&cube_thread,(pthread_attr_t *)0x0,(__start_routine *)0x35a8
                           ,(void *)0x0);
    uVar1 = 0;
    if (iVar2 != 0) {
      fprintf(_stderr,"Thread creation failed: %d\n",iVar2);
      uVar1 = 0xffffffff;
    }
  }
  return uVar1;
}



/* ===== FUNCTION cube_msg_thread_deinit @ 00013a7c ===== */

undefined4 cube_msg_thread_deinit(void)

{
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = 0xffff;
  local_c = 0;
  if (cube_thread != 0) {
    api_cube_send_msg(&local_10);
    pthread_join(cube_thread,(void **)0x0);
    cube_thread = 0;
  }
  return 0;
}



/* ===== FUNCTION cube_ioctl @ 00013ae8 ===== */

int cube_ioctl(int param_1,uint *param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 uVar1;
  uint uVar2;
  int iVar4;
  undefined4 local_res8;
  undefined4 local_resc;
  undefined2 local_28;
  undefined2 local_26;
  undefined2 local_24;
  uint *local_20;
  undefined4 *local_1c;
  byte local_18 [12];
  uint *puVar3;
  
  local_1c = &local_res8;
  local_res8 = param_3;
  local_resc = param_4;
  if (param_1 == 0x40010206) {
    local_28 = 3;
    local_26 = 0xb1;
    local_24 = 1;
    local_20 = param_2;
    iVar4 = cube_memory_get_constprop_4(&local_28);
    return -(uint)(iVar4 != 0);
  }
  if (param_1 < 0x40010207) {
    local_18[0] = (byte)param_2;
    if (param_1 == 0x20010203) {
      local_28 = CONCAT11(local_28._1_1_,local_18[0]);
      iVar4 = open64("/dev/sndC0i2so",1);
      if (iVar4 < 0) {
        return 0;
      }
      ioctl(iVar4,0x8001080b,&local_28);
      close(iVar4);
    }
    else {
      if (param_1 < 0x20010204) {
        if (param_1 == 0x20010201) {
          set_backlight_value(param_2);
          return 0;
        }
        goto LAB_00013cc8;
      }
      if (param_1 == 0x20010205) {
        local_28 = 3;
        local_26 = 0xb1;
        local_24 = 1;
        local_20 = (uint *)local_18;
        cube_memory_save_constprop_3(&local_28);
        iVar4 = api_get_hdmi_tx_connection_status(0);
        if (iVar4 == 0) {
          return 0;
        }
        fbdev_aspect_ratio_wrok((uint)param_2 & 0xff);
      }
      else {
        if (param_1 != 0x40010204) goto LAB_00013cc8;
        uVar1 = get_volume();
        *(undefined1 *)param_2 = uVar1;
      }
    }
    return 0;
  }
  if (param_1 == 0x40050207) {
    puVar3 = joykey_val_6359;
    if (joykey_val_6359 == (uint *)0x0) {
      joykey_val_6359 = (uint *)cube_shmget_init("/tmp/joy_key",8);
      if (joykey_val_6359 == (uint *)0x0) goto LAB_00013c88;
      uVar2 = *joykey_val_6359;
      goto LAB_00013c60;
    }
  }
  else {
    if (param_1 < 0x40050208) {
      if (param_1 == 0x40050202) {
        local_18[0] = 0;
        iVar4 = api_get_hdmi_tx_connection_status(0);
        if (iVar4 == 0) {
          local_28 = 1;
          local_24 = 1;
          local_20 = (uint *)local_18;
          local_26 = 0x1e;
          cube_memory_get_constprop_4(&local_28);
          *param_2 = (uint)local_18[0];
          return 0;
        }
LAB_00013c88:
        *param_2 = 0;
        return 0;
      }
LAB_00013cc8:
      printf("There is no such command %d\n","_cube_ioctl_",0x298,param_1);
      return 0;
    }
    if (param_1 != 0x40050208) {
      if (param_1 == 0x40050209) {
        if (joykey_val_6363 != 0) {
          *param_2 = joykey_val_6363;
          return 0;
        }
        joykey_val_6363 = cube_shmget_init("/tmp/joy_key",8);
        *param_2 = joykey_val_6363;
        return 0;
      }
      goto LAB_00013cc8;
    }
    puVar3 = game_status_6355;
    if (game_status_6355 == (uint *)0x0) {
      uVar2 = cube_shmget_game_status_part_1();
      *param_2 = uVar2;
      return 0;
    }
  }
  uVar2 = *puVar3;
LAB_00013c60:
  *param_2 = uVar2;
  return 0;
}



/* ===== FUNCTION api_cube_free @ 00013e50 ===== */

void api_cube_free(void *param_1)

{
  int __fd;
  
  if (-1 < mmz_fd) {
    printf("mmz free %p, %d\n",param_1);
    munmap(param_1,mmz_size);
    __fd = mmz_fd;
    mmz_size = 0;
    mmz_malloced = 0;
    ioctl(mmz_fd,0xc0101902,mmz);
    close(__fd);
    mmz_fd = -1;
  }
  return;
}



/* ===== FUNCTION api_cube_malloc @ 00013f10 ===== */

void * api_cube_malloc(size_t param_1)

{
  int __fd;
  int iVar1;
  void *pvVar2;
  undefined4 local_28;
  undefined4 local_24;
  size_t local_20;
  undefined4 local_1c;
  
  local_28 = 0;
  local_1c = 0;
  local_24 = 0x20;
  if (mmz_malloced == 0) {
    local_20 = param_1;
    __fd = open64("/dev/mmz",2);
    mmz_fd = __fd;
    if (__fd < 0) {
      puts("mmz_malloc -ENODEV");
      pvVar2 = (void *)0x0;
    }
    else {
      local_28 = 0;
      local_24 = 0x20;
      local_20 = param_1;
      iVar1 = ioctl(__fd,0xc0101901,&local_28);
      if (iVar1 < 0) {
        close(__fd);
        puts("mmz_malloc -ENOMEM");
        pvVar2 = (void *)0x0;
      }
      else {
        pvVar2 = mmap64((void *)0x0,param_1,3,1,__fd,0);
        if (pvVar2 == (void *)0xffffffff) {
          close(__fd);
          printf("%s -MAP_FAILED\n","api_cube_malloc");
        }
        else {
          mmz_malloced = 1;
          mmz_size = param_1;
          printf("mmz malloc user:0x%x\n",pvVar2);
        }
      }
    }
    return pvVar2;
  }
  return (void *)0x0;
}



/* ===== FUNCTION video_drivers_init @ 000140d0 ===== */

int video_drivers_init(void)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  
  printf("%s:%d\n","video_drivers_init",0x33);
  piVar4 = malloc(0x30);
  uVar3 = colormode;
  iVar2 = SCREEN_FB_WIDTH;
  iVar5 = SCREEN_FB_HEIGHT;
  bVar1 = colormode < 6;
  *piVar4 = SCREEN_FB_WIDTH;
  piVar4[1] = iVar5;
  frame = piVar4;
  if (bVar1) {
    uVar6 = 1 << (uVar3 & 0x1f);
    if ((uVar6 & 0x14) == 0) {
      if ((uVar6 & 0x23) == 0) {
        if ((uVar6 & 8) == 0) goto LAB_00014190;
        piVar4[3] = 4;
      }
      else {
        piVar4[3] = 2;
      }
    }
    else {
      piVar4[3] = 3;
    }
    piVar4[2] = iVar2 << 1;
    piVar4[4] = uVar3;
    printf("%s:%d, colormode: %d\n","video_drivers_init",0x4a);
    fbdev_init(SCREEN_FB_WIDTH,SCREEN_FB_HEIGHT,colormode);
                    /* WARNING: Could not recover jumptable at 0x0001423c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar5 = printf("%s:%d, colormode: %d\n","video_drivers_init",0x4c,colormode);
    return iVar5;
  }
LAB_00014190:
  printf("Not support color format %d.\n",uVar3);
  return -1;
}



/* ===== FUNCTION video_driver_deinit @ 00014250 ===== */

void video_driver_deinit(void)

{
  fbdev_deinit();
  return;
}



/* ===== FUNCTION video_driver_mutex_lock @ 00014280 ===== */

void video_driver_mutex_lock(void)

{
  if (frame == 0) {
    return;
  }
  (*(code *)(undefined *)0x0)(frame + 0x18);
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
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION video_driver_mutex_unlock @ 000142b0 ===== */

void video_driver_mutex_unlock(void)

{
  if (frame == 0) {
    return;
  }
  (*(code *)(undefined *)0x0)(frame + 0x18);
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION video_driver_disp_frame @ 000142e0 ===== */

void video_driver_disp_frame(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  video_driver_mutex_lock();
  piVar2 = frame;
  iVar1 = colormode;
  if ((((*frame == param_2) && (frame[1] == param_3)) && (iVar3 = frame[2], iVar3 == param_4)) &&
     (frame[4] == colormode)) goto LAB_000143ec;
  *frame = param_2;
  piVar2[1] = param_3;
  piVar2[2] = param_4;
  piVar2[4] = iVar1;
  if (param_2 == 0x500) {
    if (param_3 != 0x2d0) goto LAB_00014378;
    SCREEN_FB_WIDTH = 0x500;
    SCREEN_FB_HEIGHT = 0x2d0;
  }
  else if (param_2 == 0x356) {
    SCREEN_FB_HEIGHT = 0x356;
    SCREEN_FB_WIDTH = param_3;
  }
  else {
LAB_00014378:
    SCREEN_FB_WIDTH = 0x280;
    SCREEN_FB_HEIGHT = 0x1e0;
  }
  fbdev_deinit();
  fbdev_init(SCREEN_FB_WIDTH,SCREEN_FB_HEIGHT,colormode);
  printf("video_driver_setting %d %d %d img_w:%d img_h:%d img_p:%d\n",SCREEN_FB_WIDTH,
         SCREEN_FB_HEIGHT,colormode,param_2,param_3,param_4);
  iVar3 = param_4;
LAB_000143ec:
  frame[5] = param_1;
  fbdev_draw_frame(param_1,0,0,param_2,param_3,iVar3);
                    /* WARNING: Could not recover jumptable at 0x0001443c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  video_driver_mutex_unlock();
  return;
}



/* ===== FUNCTION video_driver_setting @ 000144ac ===== */

undefined4 video_driver_setting(undefined4 *param_1)

{
  int iVar1;
  
  SCREEN_FB_HEIGHT = param_1[3];
  iVar1 = param_1[4];
  colormode = *param_1;
  if (SCREEN_FB_HEIGHT <= iVar1) {
    SCREEN_FB_WIDTH = SCREEN_FB_HEIGHT;
    SCREEN_FB_HEIGHT = iVar1;
    return 1;
  }
  SCREEN_FB_WIDTH = iVar1;
  return 1;
}



/* ===== FUNCTION video_driver_setmode @ 00014504 ===== */

void video_driver_setmode(uint param_1)

{
  if ((param_1 & 0xff00) == 0xff00) {
    printf("video_driver_set_rotation %d\n",param_1 & 0xff);
                    /* WARNING: Could not recover jumptable at 0x000145b4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    fbdev_setrotate(param_1 & 0xff);
    return;
  }
  if ((param_1 & 0x100) == 0) {
    puts("video_driver_set_aspect_ratio 4:3");
                    /* WARNING: Could not recover jumptable at 0x00014558. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    fbdev_video_aspect_ratio(1);
    return;
  }
  puts("video_driver_set_aspect_ratio 16:9");
                    /* WARNING: Could not recover jumptable at 0x00014580. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  fbdev_video_aspect_ratio(0);
  return;
}



/* ===== FUNCTION video_driver_get_size @ 000145bc ===== */

void video_driver_get_size(undefined4 *param_1,undefined4 *param_2)

{
  *param_1 = SCREEN_FB_WIDTH;
  *param_2 = SCREEN_FB_HEIGHT;
  return;
}



/* ===== FUNCTION video_driver_get_screen_size @ 000145e4 ===== */

void video_driver_get_screen_size(undefined4 *param_1,undefined4 *param_2)

{
  *param_1 = SCREEN_FB_WIDTH;
  *param_2 = SCREEN_FB_HEIGHT;
  return;
}



/* ===== FUNCTION video_driver_update_screen @ 0001460c ===== */

void video_driver_update_screen(void)

{
  if (frame[5] != 0) {
    fbdev_draw_frame(frame[5],0,0,*frame,frame[1],frame[2]);
  }
  return;
}



/* ===== FUNCTION set_bootup_slot @ 00014670 ===== */

undefined4 set_bootup_slot(undefined4 param_1)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open64("/dev/standby",2);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    ioctl(__fd,0x20002709,param_1);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION key_task @ 000146fc ===== */

void key_task(void)

{
  ssize_t sVar1;
  undefined1 auStack_30 [8];
  int local_28;
  
  do {
    if ((-1 < fd_ir) && (sVar1 = read(fd_ir,auStack_30,0x10), sVar1 == 0x10)) {
      if (local_28 == 0xae0001) {
        puts("exit game\r");
        exit_key_pressed = 1;
      }
      if (local_28 == 0x2090001) {
        puts("exit game\r");
        exit_key_pressed = 1;
      }
    }
    usleep(500000);
  } while( true );
}



/* ===== FUNCTION exit_game @ 000147e4 ===== */

void exit_game(void)

{
  int iVar1;
  stat64 sStack_a8;
  
  printf("%s:%d\r\n","exit_game",0x31);
  set_bootup_slot(0x11);
  iVar1 = __xstat64(3,"/mnt/sdcard/cubegm/avexit",&sStack_a8);
  if (iVar1 != -1) {
    puts("avexit\r");
    system("/mnt/sdcard/cubegm/avexit");
  }
  system("reboot -f");
  return;
}



/* ===== FUNCTION get_multios_app @ 0001489c ===== */

undefined4 get_multios_app(void)

{
  int __fd;
  int iVar1;
  undefined1 auStack_120 [108];
  undefined4 local_b4;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined1 *local_14;
  
  memset(auStack_120,0,0x104);
  __fd = open64("/dev/persistentmem",2);
  if (__fd < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",__fd);
    local_b4 = 0xffffffff;
  }
  else {
    local_1a = 0;
    local_18 = 0x104;
    local_1c = 3;
    local_14 = auStack_120;
    iVar1 = ioctl(__fd,0x400c2602,&local_1c);
    if (iVar1 < 0) {
      puts("get avparam node fail");
      close(__fd);
      local_b4 = 0xffffffff;
    }
    else {
      close(__fd);
    }
  }
  return local_b4;
}



/* ===== FUNCTION set_multios_app @ 00014998 ===== */

undefined4 set_multios_app(int param_1,uint param_2,char *param_3)

{
  int __fd;
  int iVar1;
  size_t sVar2;
  size_t __n;
  undefined4 uVar3;
  byte abStack_138 [108];
  int local_cc;
  byte local_c8;
  char local_c7 [147];
  undefined2 local_34;
  undefined2 local_32;
  undefined2 local_30;
  byte *local_2c;
  
  memset(abStack_138,0,0x104);
  __fd = open64("/dev/persistentmem",2);
  if (__fd < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",__fd);
    uVar3 = 0xffffffff;
  }
  else {
    local_32 = 0;
    local_30 = 0x104;
    local_34 = 3;
    local_cc = param_1;
    local_c8 = (byte)param_2;
    local_2c = abStack_138;
    iVar1 = ioctl(__fd,0x400c2602,&local_34);
    if (iVar1 < 0) {
      puts("get avparam node fail");
      close(__fd);
      uVar3 = 0xffffffff;
    }
    else {
      if (param_1 != local_cc) {
        local_32 = 0x6c;
        local_34 = 3;
        local_30 = 4;
        local_2c = (byte *)&local_cc;
        local_cc = param_1;
        ioctl(__fd,0x800c2603,&local_34);
      }
      if (param_2 != local_c8) {
        local_34 = 3;
        local_32 = 0x70;
        local_30 = 1;
        local_2c = &local_c8;
        local_c8 = (byte)param_2;
        ioctl(__fd,0x800c2603,&local_34);
      }
      if (param_3 != (char *)0x0) {
        sVar2 = strlen(local_c7);
        __n = strlen(param_3);
        if (__n <= sVar2) {
          __n = sVar2;
        }
        iVar1 = strncmp(local_c7,param_3,__n);
        if (iVar1 != 0) {
          memset(local_c7,0,0x40);
          strncpy(local_c7,param_3,0x40);
          local_30 = 0x40;
          local_34 = 3;
          local_32 = 0x71;
          local_2c = (byte *)local_c7;
          ioctl(__fd,0x800c2603,&local_34);
        }
      }
      local_32 = 0;
      local_34 = 3;
      local_30 = 0x104;
      local_2c = abStack_138;
      iVar1 = ioctl(__fd,0x400c2602,&local_34);
      if (iVar1 == 0) {
        printf("set app to : %d\r\n",local_cc);
        printf("set language to : %d\r\n",(uint)local_c8);
        if (local_c7[0] != '\0') {
          printf("set keymap to : %s\r\n",local_c7);
        }
      }
      close(__fd);
      uVar3 = 0;
    }
  }
  return uVar3;
}



/* ===== FUNCTION api_osd_show_onoff @ 00014cb4 ===== */

undefined4 api_osd_show_onoff(uint param_1)

{
  int __fd;
  int iVar1;
  
  __fd = open64("/dev/fb0",2);
  if (__fd == -1) {
    printf("%s(), line: %d. Error: cannot open framebuffer device","api_osd_show_onoff",0xb7);
  }
  else {
    iVar1 = ioctl(__fd,0x4611,param_1 ^ 1);
    if (iVar1 != 0) {
      printf("%s(), line: %d. Error: FBIOBLANK","api_osd_show_onoff",0xc1);
    }
    close(__fd);
  }
  return 0;
}



/* ===== FUNCTION enter_multios_app @ 00014d84 ===== */

undefined4 enter_multios_app(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  stat64 sStack_b0;
  
  printf("%s:%d\r\n","enter_multios_app",0xf7);
  fbdev_deinit();
  sound_driver_deinit();
  iVar1 = set_multios_app(param_1,param_2,param_3);
  if (iVar1 < 0) {
    puts("Failed to set multios app\r");
  }
  set_bootup_slot(0x21);
  iVar1 = __xstat64(3,"/mnt/sdcard/cubegm/avexit",&sStack_b0);
  if (iVar1 != -1) {
    puts("avexit\r");
    system("/mnt/sdcard/cubegm/avexit");
  }
  fbdev_aspect_ratio_wrok(0);
  api_osd_show_onoff(0);
  system("/mnt/sdcard/cubegm/icubemp_start.sh &");
  return 0;
}



/* ===== FUNCTION check_exit_game @ 00014ed0 ===== */

/* WARNING: Heritage AFTER dead removal. Example location: r0x00033630 : 0x00014fbc */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4 check_exit_game(void)

{
  pthread_attr_t pStack_38;
  
  if (event_inited != 0) {
    return exit_key_pressed;
  }
  fd_ir = open64("/dev/input/event0",0);
  open64("/dev/check_adc1",0);
  if (-1 < fd_ir) {
    pthread_attr_init(&pStack_38);
    pthread_attr_setstacksize(&pStack_38,0x2000);
    pthread_create(&key_thread,&pStack_38,(__start_routine *)0x46fc,(void *)0x0);
  }
  event_inited = 1;
  return exit_key_pressed;
}



/* ===== FUNCTION fb_render_task @ 00014fd0 ===== */

void fb_render_task(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  pthread_mutex_t *__mutex;
  undefined1 auStack_28 [8];
  
  __mutex = (pthread_mutex_t *)(param_1 + 0x389);
  do {
    if (param_1[1] != 0) {
      return;
    }
    pthread_mutex_lock(__mutex);
    iVar3 = param_1[0x47];
    iVar2 = iVar3 + 9;
    if (param_1[iVar2 * 8 + 2] == 0) {
      pthread_cond_wait((pthread_cond_t *)(param_1 + 0x390),__mutex);
      iVar3 = param_1[0x47];
      iVar2 = iVar3 + 9;
      if (param_1[iVar2 * 8 + 2] != 0) {
        iVar1 = param_1[0x45];
        goto LAB_00015024;
      }
    }
    else {
      iVar1 = param_1[0x45];
LAB_00015024:
      param_1[0x47] = iVar3 + 1;
      if (iVar3 + 1 == iVar1) {
        param_1[0x47] = 0;
      }
      pthread_mutex_unlock(__mutex);
      hcge_engine_sync(param_1[0x44]);
      param_1[0x1c] = param_1[0x18] * iVar3;
      ioctl(*param_1,0x4606,param_1 + 0x17);
      ioctl(*param_1,0x80044620,auStack_28);
      pthread_mutex_lock(__mutex);
      if (param_1[0x46] != -1) {
        param_1[(param_1[0x46] + 9) * 8 + 2] = 0;
      }
      param_1[0x46] = iVar3;
      param_1[iVar2 * 8 + 2] = 2;
      pthread_cond_signal((pthread_cond_t *)(param_1 + 0x39c));
    }
    pthread_mutex_unlock(__mutex);
  } while( true );
}



/* ===== FUNCTION __fbdev_deinit @ 00015190 ===== */

void __fbdev_deinit(int *param_1)

{
  int __fd;
  
  if (param_1 == (int *)0x0) {
    return;
  }
  param_1[1] = 1;
  pthread_cond_signal((pthread_cond_t *)(param_1 + 0x390));
  pthread_cond_signal((pthread_cond_t *)(param_1 + 0x39c));
  pthread_cond_signal((pthread_cond_t *)(param_1 + 0x3a8));
  pthread_join(param_1[0x3b4],(void **)0x0);
  pthread_join(param_1[0x3b5],(void **)0x0);
  pthread_cond_destroy((pthread_cond_t *)(param_1 + 0x390));
  pthread_cond_destroy((pthread_cond_t *)(param_1 + 0x39c));
  pthread_mutex_destroy((pthread_mutex_t *)(param_1 + 0x389));
  if (param_1[0x44] != 0) {
    hcge_close();
  }
  __fd = *param_1;
  if (0 < __fd) {
    if ((void *)param_1[5] != (void *)0x0) {
      munmap((void *)param_1[5],param_1[0xb]);
      __fd = *param_1;
    }
    close(__fd);
  }
  (*(code *)(undefined *)0x0)(param_1);
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION fb_paint_task @ 000152d8 ===== */

void fb_paint_task(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint *puVar11;
  uint uVar12;
  pthread_mutex_t *__mutex;
  undefined4 *__s;
  int iVar13;
  int iVar14;
  uint uVar15;
  uint uVar16;
  int iVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  double dVar25;
  uint local_88;
  uint local_84;
  uint local_80;
  uint local_7c;
  uint local_78;
  uint local_74;
  int local_70;
  int local_6c;
  uint *local_68;
  uint *local_64;
  uint local_60;
  int local_5c;
  uint local_58;
  uint local_54;
  uint local_50;
  
  dVar22 = 2147483648.0;
  dVar23 = 4294967296.0;
  local_64 = &switchD_000154dc::switchdataD_000214b0;
  dVar24 = dVar22;
  dVar25 = dVar23;
  do {
    __mutex = (pthread_mutex_t *)(param_1 + 0xe24);
    do {
      do {
        if (*(int *)(param_1 + 4) != 0) {
          return;
        }
        pthread_mutex_lock(__mutex);
        if (*(int *)(param_1 + 0xda8) == 0) {
          pthread_cond_wait((pthread_cond_t *)(param_1 + 0xe70),__mutex);
        }
        pthread_mutex_unlock(__mutex);
      } while (*(int *)(param_1 + 0xda8) == 0);
      pthread_mutex_lock(__mutex);
      if (*(int *)(param_1 + (*(int *)(param_1 + 0x120) + 9) * 0x20 + 8) != 0) {
        pthread_cond_wait((pthread_cond_t *)(param_1 + 0xe70),__mutex);
      }
      pthread_mutex_unlock(__mutex);
    } while (*(int *)(param_1 + (*(int *)(param_1 + 0x120) + 9) * 0x20 + 8) != 0);
    cacheflush(*(undefined4 *)(param_1 + 0xdc0),
               (*(int *)(param_1 + 0xdb0) + *(int *)(param_1 + 0xdb8)) * *(int *)(param_1 + 0xdbc),2
              );
    iVar3 = g_render;
    if (g_render != 0) {
      __s = *(undefined4 **)(g_render + 0x110);
      local_60 = *(uint *)(param_1 + 0xdac);
      local_58 = *(uint *)(param_1 + 0xdb0);
      local_68 = *(uint **)(param_1 + 0xdc0);
      iVar14 = *(int *)(param_1 + 0xdb4);
      iVar13 = *(int *)(param_1 + 0xdb8);
      local_5c = *(int *)(param_1 + *(int *)(param_1 + 0x120) * 0x20 + 0x140);
      uVar15 = *(uint *)(param_1 + 0xdbc);
      iVar17 = *(int *)(g_render + 0xedc);
      memset(__s,0,0xe0);
      iVar4 = *(int *)(iVar3 + 0xed8);
      __s[0x28] = 5;
      local_74 = local_58;
      local_78 = local_60;
      __s[0x32] = 0;
      __s[3] = 0;
      __s[1] = 0;
      *__s = 0;
      __s[0x29] = 1;
      local_70 = iVar14;
      local_6c = iVar13;
      if (iVar4 == 3) {
        iVar2 = *(int *)(iVar3 + 0xedc);
        iVar8 = *(int *)(iVar3 + 0x100);
        iVar9 = *(int *)(iVar3 + 0x14);
        iVar7 = *(int *)(iVar3 + 0x28);
        iVar4 = *(int *)(iVar3 + 0xe00);
        local_54 = local_58;
        uVar6 = *(undefined4 *)(iVar3 + 0x110);
        __s[0x2f] = uVar15;
        __s[0x13] = iVar14;
        __s[0x14] = iVar13;
        local_58 = local_58 + iVar13;
        __s[0x2e] = ((int)local_68 - iVar9) + iVar7;
        __s[0x2c] = (iVar4 - iVar9) + iVar7;
        __s[0x19] = local_58;
        __s[0x2d] = iVar8 * iVar2;
        __s[0x18] = uVar15 / 3;
        __s[2] = 0x10000;
        __s[0x17] = 0x300c02;
        __s[0x12] = 0x418c04;
        hcge_set_state(uVar6,uVar6,0x10000);
        local_68 = &local_78;
        hcge_blit(*(undefined4 *)(iVar3 + 0x110),local_68,local_60,local_54);
        puVar11 = *(uint **)(iVar3 + 0xe00);
        uVar15 = (int)(uVar15 << 2) / 3;
        uVar12 = local_58;
      }
      else {
        puVar11 = local_68;
        uVar12 = local_58 + iVar13;
        local_68 = &local_78;
      }
      if (*(uint *)(iVar3 + 0xee0) < 6) {
        switch((uint)(&_gp + local_64[*(uint *)(iVar3 + 0xee0)]) & 0xfffffffe) {
        case 0x154e4:
          __s[0x12] = 0x418c04;
          __s[0x17] = 0x40001e;
          break;
        case 0x159a0:
          __s[0x12] = 0x418c04;
          __s[0x17] = 0x400c03;
          break;
        case 0x159bc:
          __s[0x12] = 0x200801;
          __s[0x17] = 0x200801;
          break;
        case 0x159d0:
          __s[0x12] = 0x211780;
          __s[0x17] = 0x20079c;
          break;
        case 0x159ec:
          __s[0x12] = 0x211780;
          __s[0x17] = 0x20079d;
        }
      }
      iVar2 = *(int *)(iVar3 + 8);
      iVar4 = iVar13;
      uVar16 = uVar15;
      if (iVar2 == 0) {
        iVar2 = 0;
        iVar13 = iVar14;
      }
      else {
        if (iVar17 == 0) {
          trap(7);
        }
        iVar9 = *(int *)(iVar3 + 0x14);
        iVar8 = *(int *)(iVar3 + 0xde0);
        iVar7 = *(int *)(iVar3 + 0x28);
        __s[0x13] = iVar13;
        __s[0x14] = iVar14;
        __s[0x19] = uVar12;
        __s[0x2f] = uVar15;
        __s[0x2e] = ((int)puVar11 - iVar9) + iVar7;
        __s[0x2c] = (iVar8 - iVar9) + iVar7;
        __s[0x32] = 0;
        __s[2] = 0x10000;
        local_54 = iVar13 * iVar17;
        __s[0x18] = (int)uVar15 / iVar17;
        __s[0x2d] = local_54;
        if (iVar2 == 0x5a) {
          __s[3] = 0x2000;
        }
        else if (iVar2 == 0xb4) {
          __s[0x2d] = uVar15;
          __s[3] = 0x1000;
        }
        else if (iVar2 == 0x10e) {
          __s[3] = 0x4000;
        }
        local_58 = uVar12;
        local_50 = local_54;
        hcge_set_state(*(undefined4 *)(iVar3 + 0x110),*(undefined4 *)(iVar3 + 0x110),0x10000);
        hcge_blit(*(undefined4 *)(iVar3 + 0x110),local_68,0,0);
        iVar2 = *(int *)(iVar3 + 8);
        puVar11 = *(uint **)(iVar3 + 0xde0);
        if (iVar2 == 0xb4) {
          __s[0x17] = __s[0x12];
          uVar12 = local_58;
          iVar13 = iVar14;
        }
        else {
          local_78 = 0;
          local_74 = 0;
          __s[0x17] = __s[0x12];
          uVar12 = local_60 + iVar14;
          uVar15 = local_50;
          iVar4 = iVar14;
          uVar16 = local_54;
          local_70 = iVar13;
          local_6c = iVar14;
        }
      }
      if ((*(short *)(iVar3 + 0xc) != 0) && (*(int *)(iVar3 + 0xe20) != 0)) {
        iVar14 = *(int *)(iVar3 + 0x14);
        iVar8 = *(int *)(iVar3 + 0x28);
        __s[0x2c] = (*(int *)(iVar3 + 0xe20) - iVar14) + iVar8;
        if (iVar2 == 0) {
          if (iVar17 == 0) {
            trap(7);
          }
          __s[0x19] = uVar12;
          __s[0x2d] = uVar15;
          __s[0x18] = (int)uVar16 / iVar17;
        }
        else {
          __s[0x18] = __s[0x13];
          __s[0x19] = __s[0x14];
        }
        cVar1 = *(char *)(iVar3 + 0xd);
        __s[3] = 0;
        __s[0x2f] = uVar15;
        __s[0x2e] = ((int)puVar11 - iVar14) + iVar8;
        if (cVar1 != '\0') {
          __s[3] = 0x1000000;
        }
        if (*(char *)(iVar3 + 0xc) != '\0') {
          __s[3] = __s[3] | 0x2000000;
        }
        uVar6 = *(undefined4 *)(iVar3 + 0x110);
        __s[2] = 0x10000;
        local_60 = uVar12;
        local_58 = uVar15;
        hcge_set_state(uVar6,uVar6,0x10000);
        hcge_blit(*(undefined4 *)(iVar3 + 0x110),local_68,0,0);
        puVar11 = *(uint **)(iVar3 + 0xe20);
        uVar12 = local_60;
        uVar15 = local_58;
      }
      if (*(int *)(iVar3 + 0x10) == 0) {
        dVar19 = (double)*(int *)(iVar3 + 0x108);
        if (*(int *)(iVar3 + 0x108) < 0) {
          dVar19 = dVar19 + 4294967296.0;
        }
        dVar20 = (double)*(int *)(iVar3 + 0x10c);
        dVar18 = dVar19 / (double)iVar13;
        if (*(int *)(iVar3 + 0x10c) < 0) {
          dVar20 = dVar20 + 4294967296.0;
        }
        dVar21 = dVar20 / (double)iVar4;
        local_84 = 0;
        if (dVar21 < dVar18) {
          uVar10 = *(uint *)(iVar3 + 0x100);
          dVar20 = (double)(int)uVar10;
          if ((int)uVar10 < 0) {
            dVar20 = dVar20 + dVar25;
          }
          dVar19 = ((double)iVar13 * dVar21 * dVar20) / dVar19;
          if (dVar19 < dVar24) {
            local_80 = (uint)dVar19;
          }
          else {
            local_80 = (int)(dVar19 - dVar24) | 0x80000000;
          }
          uVar5 = *(uint *)(iVar3 + 0x104);
          local_88 = uVar10 - local_80 >> 1;
          local_7c = uVar5;
        }
        else {
          local_88 = 0;
          if (dVar18 < dVar21) {
            uVar5 = *(uint *)(iVar3 + 0x104);
            uVar10 = *(uint *)(iVar3 + 0x100);
            dVar19 = (double)(int)uVar5;
            if ((int)uVar5 < 0) {
              dVar19 = dVar19 + dVar23;
            }
            dVar20 = (dVar18 * (double)iVar4 * dVar19) / dVar20;
            if (dVar20 < dVar22) {
              local_7c = (uint)dVar20;
            }
            else {
              local_7c = (int)(dVar20 - dVar22) | 0x80000000;
            }
            local_88 = 0;
            local_84 = uVar5 - local_7c >> 1;
            local_80 = uVar10;
          }
          else {
            uVar10 = *(uint *)(iVar3 + 0x100);
            uVar5 = *(uint *)(iVar3 + 0x104);
            local_80 = uVar10;
            local_7c = uVar5;
          }
        }
      }
      else {
        uVar10 = *(uint *)(iVar3 + 0x100);
        uVar5 = *(uint *)(iVar3 + 0x104);
        local_88 = 0;
        local_84 = 0;
        local_80 = uVar10;
        local_7c = uVar5;
      }
      if (iVar17 == 0) {
        trap(7);
      }
      iVar4 = *(int *)(iVar3 + 0x14);
      iVar13 = *(int *)(iVar3 + 0x28);
      uVar6 = *(undefined4 *)(iVar3 + 0x110);
      __s[0x14] = uVar5;
      __s[0x13] = uVar10;
      __s[0x19] = uVar12;
      __s[0x2e] = ((int)puVar11 - iVar4) + iVar13;
      __s[0x2f] = uVar15;
      __s[0x32] = 4;
      __s[0x35] = uVar10 - 1;
      __s[2] = 0x20000;
      __s[0x36] = uVar5 - 1;
      __s[3] = 0;
      __s[0x2c] = (local_5c - iVar4) + iVar13;
      __s[0x33] = 0;
      __s[0x34] = 0;
      __s[0x18] = (int)uVar16 / iVar17;
      __s[0x2d] = iVar17 * uVar10;
      hcge_set_state(uVar6,uVar6,0x20000);
      hcge_stretch_blit(*(undefined4 *)(iVar3 + 0x110),local_68,&local_88);
    }
    pthread_mutex_lock(__mutex);
    *(undefined4 *)(param_1 + 0xda8) = 0;
    iVar3 = *(int *)(param_1 + 0x120) + 1;
    iVar13 = param_1 + *(int *)(param_1 + 0x120) * 0x20;
    *(undefined4 *)(iVar13 + 0x128) = 1;
    *(undefined4 *)(iVar13 + 0x124) = *(undefined4 *)(param_1 + 0xda4);
    *(int *)(param_1 + 0x120) = iVar3;
    if (iVar3 == *(int *)(param_1 + 0x114)) {
      *(undefined4 *)(param_1 + 0x120) = 0;
    }
    pthread_cond_signal((pthread_cond_t *)(param_1 + 0xe40));
    pthread_cond_signal((pthread_cond_t *)(param_1 + 0xea0));
    pthread_mutex_unlock(__mutex);
  } while( true );
}



/* ===== FUNCTION driver_get_param_from_dts @ 00015b94 ===== */

int driver_get_param_from_dts(char *param_1)

{
  int __fd;
  ssize_t sVar1;
  uint local_10 [2];
  
  __fd = open64(param_1,0);
  if (__fd < 0) {
    return 0;
  }
  sVar1 = read(__fd,local_10,4);
  if (sVar1 != 4) {
    close(__fd);
    return 0;
  }
  close(__fd);
  return (((local_10[0] >> 0x10) << 0x18 | (local_10[0] >> 0x18) << 0x10) >> 0x10) +
         ((local_10[0] & 0xff) << 8 | local_10[0] >> 8 & 0xff) * 0x10000;
}



/* ===== FUNCTION driver_get_string_from_dts.constprop.1 @ 00015c44 ===== */

void driver_get_string_from_dts_constprop_1(char *param_1,void *param_2)

{
  int __fd;
  
  __fd = open64(param_1,0);
  if (-1 < __fd) {
    read(__fd,param_2,0x10);
                    /* WARNING: Could not recover jumptable at 0x00015cac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    close(__fd);
    return;
  }
  return;
}



/* ===== FUNCTION fbdev_draw_frame @ 00015cc8 ===== */

void fbdev_draw_frame(int param_1,int param_2,int param_3,uint param_4,uint param_5,int param_6)

{
  int iVar1;
  uint uVar2;
  pthread_mutex_t *__mutex;
  size_t sVar3;
  uint uVar4;
  void *pvVar5;
  int iVar6;
  int iVar7;
  void *__src;
  int iVar8;
  int iVar9;
  
  iVar1 = g_render;
  if (g_render == 0) {
    return;
  }
  uVar4 = *(uint *)(g_render + 0x100);
  uVar2 = *(uint *)(g_render + 0x104);
  if (((uVar4 <= uVar2) || ((param_4 <= uVar4 && (param_5 <= uVar2)))) &&
     ((uVar2 <= uVar4 || ((param_4 <= uVar2 && (param_5 <= uVar4)))))) {
    __mutex = (pthread_mutex_t *)(g_render + 0xe24);
    iVar8 = param_3 + param_5;
    pthread_mutex_lock(__mutex);
    if (*(int *)(iVar1 + 0xdc0) == 0) {
      uVar2 = *(uint *)(iVar1 + 0x100);
      if (((*(int *)(iVar1 + 8) == 0) && (uVar2 == param_4)) &&
         (*(uint *)(iVar1 + 0x104) == param_5)) {
        iVar7 = *(int *)(iVar1 + 0x114);
        iVar9 = *(int *)(iVar1 + 0x120);
        if ((iVar7 < 2) || (*(int *)(iVar1 + (iVar9 + 9) * 0x20 + 8) == 0)) {
          if (param_3 < iVar8) {
            sVar3 = uVar2 * *(int *)(iVar1 + 0xed8);
            __src = (void *)(param_1 + param_2 * *(int *)(iVar1 + 0xed8) + param_3 * param_6);
            pvVar5 = (void *)0x0;
            do {
              param_3 = param_3 + 1;
              pvVar5 = memcpy(pvVar5,__src,sVar3);
              pvVar5 = (void *)((int)pvVar5 + sVar3);
              __src = (void *)((int)__src + param_6);
            } while (iVar8 != param_3);
          }
          iVar6 = iVar9 + 1;
          iVar8 = iVar1 + iVar9 * 0x20;
          if (iVar7 == iVar6) {
            iVar6 = 0;
          }
          *(undefined4 *)(iVar8 + 0x128) = 1;
          *(int *)(iVar8 + 0x124) = g_order;
          *(int *)(iVar1 + 0x120) = iVar6;
          g_order = g_order + 1;
          pthread_cond_signal((pthread_cond_t *)(iVar1 + 0xe40));
        }
        else {
          puts("drop frame");
        }
      }
      else {
        printf("Not support screen is %dx%d rotate disabled, request %dx%d rotate %d\n",uVar2,
               *(uint *)(iVar1 + 0x104),param_4,param_5,*(int *)(iVar1 + 8));
      }
    }
    else {
      if ((*(int *)(iVar1 + 0xda8) != 0) &&
         ((pthread_cond_wait((pthread_cond_t *)(iVar1 + 0xea0),__mutex), *(int *)(iVar1 + 4) != 0 ||
          (*(int *)(iVar1 + 0xda8) != 0)))) {
        pthread_mutex_unlock(__mutex);
        goto code_r0x00020d80;
      }
      pthread_mutex_unlock(__mutex);
      pvVar5 = *(void **)(iVar1 + 0xdc0);
      if (param_3 < iVar8) {
        iVar7 = *(int *)(iVar1 + 0xed8);
        iVar9 = param_3 * param_6;
        sVar3 = param_4 * iVar7;
        do {
          param_3 = param_3 + 1;
          iVar7 = iVar7 * param_2 + iVar9;
          iVar9 = iVar9 + param_6;
          pvVar5 = memcpy(pvVar5,(void *)(param_1 + iVar7),sVar3);
          iVar7 = *(int *)(iVar1 + 0xed8);
          sVar3 = param_4 * iVar7;
          pvVar5 = (void *)((int)pvVar5 + sVar3);
        } while (iVar8 != param_3);
      }
      else {
        sVar3 = param_4 * *(int *)(iVar1 + 0xed8);
      }
      *(undefined4 *)(iVar1 + 0xdac) = 0;
      *(undefined4 *)(iVar1 + 0xdb0) = 0;
      *(uint *)(iVar1 + 0xdb4) = param_4;
      *(uint *)(iVar1 + 0xdb8) = param_5;
      *(size_t *)(iVar1 + 0xdbc) = sVar3;
      pthread_mutex_lock(__mutex);
      *(undefined4 *)(iVar1 + 0xda8) = 1;
      *(int *)(iVar1 + 0xda4) = g_order;
      g_order = g_order + 1;
      pthread_cond_signal((pthread_cond_t *)(iVar1 + 0xe70));
    }
    (*(code *)(undefined *)0x0)(__mutex);
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
  }
code_r0x00020d80:
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION fbdev_backlight_init @ 00016070 ===== */

void fbdev_backlight_init(undefined4 param_1)

{
  int __fd;
  undefined4 local_18 [3];
  
  __fd = open64("/dev/backlight",2);
  if (-1 < __fd) {
    local_18[0] = param_1;
    write(__fd,local_18,4);
    fsync(__fd);
    close(__fd);
    return;
  }
  puts("open backlight failed");
  return;
}



/* ===== FUNCTION fbdev_video_aspect_ratio @ 00016124 ===== */

void fbdev_video_aspect_ratio(int param_1)

{
  ge_is_full_screen = (uint)(param_1 == 0);
  (*(code *)(undefined *)0x0)("is_full_screen:%d\n",param_1);
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION fbdev_deinit @ 00016150 ===== */

void fbdev_deinit(void)

{
  if (g_render != 0) {
    g_render = 0;
    __fbdev_deinit();
    return;
  }
  return;
}



/* ===== FUNCTION get_screen_rotation_from_dtb @ 00016184 ===== */

undefined4 get_screen_rotation_from_dtb(undefined2 *param_1,undefined1 *param_2,undefined1 *param_3)

{
  undefined1 uVar1;
  undefined2 uVar2;
  int iVar3;
  undefined4 uVar4;
  char local_28 [16];
  
  local_28[0] = '\0';
  local_28[1] = '\0';
  local_28[2] = '\0';
  local_28[3] = '\0';
  local_28[4] = '\0';
  local_28[5] = '\0';
  local_28[6] = '\0';
  local_28[7] = '\0';
  local_28[8] = '\0';
  local_28[9] = '\0';
  local_28[10] = '\0';
  local_28[0xb] = '\0';
  local_28[0xc] = '\0';
  local_28[0xd] = '\0';
  local_28[0xe] = '\0';
  local_28[0xf] = '\0';
  driver_get_string_from_dts_constprop_1("/proc/device-tree/hcrtos/rotate/status",local_28);
  iVar3 = strcmp(local_28,"okay");
  if (iVar3 == 0) {
    uVar2 = driver_get_param_from_dts("/proc/device-tree/hcrtos/rotate/rotate");
    *param_1 = uVar2;
    uVar1 = driver_get_param_from_dts("/proc/device-tree/hcrtos/rotate/h_flip");
    *param_2 = uVar1;
    uVar1 = driver_get_param_from_dts("/proc/device-tree/hcrtos/rotate/v_flip");
    *param_3 = uVar1;
    uVar4 = 0;
  }
  else {
    uVar4 = 0xffffffff;
  }
  return uVar4;
}



/* ===== FUNCTION fbdev_get_avparam @ 00016284 ===== */

void fbdev_get_avparam(void)

{
  byte bVar1;
  int iVar2;
  char *__fd;
  int iVar3;
  char *pcVar4;
  code *pcVar5;
  char local_50 [4];
  undefined4 local_4c;
  undefined1 *local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined2 local_3c;
  undefined2 uStack_3a;
  undefined4 *local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  
  local_50[0] = '\0';
  local_50[1] = '\0';
  local_50[2] = '\0';
  local_50[3] = '\0';
  local_4c = 0;
  local_48 = (undefined1 *)0x0;
  local_44 = 0;
  local_2c = 0;
  local_28 = 0;
  iVar2 = get_screen_rotation_from_dtb((int)&local_2c + 2,&local_28,(int)&local_28 + 1);
  if (iVar2 == 0) {
    local_2c._0_2_ = (ushort)(byte)local_2c;
  }
  driver_get_string_from_dts_constprop_1("/proc/device-tree/hcrtos/i2so/status",local_50);
  iVar2 = strcmp(local_50,"okay");
  if (iVar2 == 0) {
    bVar1 = driver_get_param_from_dts("/proc/device-tree/hcrtos/i2so/volume");
    if (100 < bVar1) {
      bVar1 = 100;
    }
    local_2c = CONCAT31(local_2c._1_3_,bVar1);
  }
  local_50[0] = '\0';
  local_50[1] = '\0';
  local_50[2] = '\0';
  local_50[3] = '\0';
  local_4c = 0;
  local_48 = (undefined1 *)0x0;
  local_44 = 0;
  driver_get_string_from_dts_constprop_1("/proc/device-tree/hcrtos/hdmi/status",local_50);
  iVar2 = strcmp(local_50,"okay");
  if (iVar2 == 0) {
    iVar2 = api_get_hdmi_tx_connection_status(0);
    if (iVar2 != 0) {
      local_2c = local_2c & 0xffff;
      local_28 = local_28 & 0xffff0000;
    }
  }
  local_40 = 0;
  local_3c = 0;
  uStack_3a = 0;
  local_38 = (undefined4 *)0x0;
  local_34 = 0;
  local_30 = 0;
  local_24 = 0;
  __fd = (char *)open64("/dev/persistentmem",2);
  if ((int)__fd < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",__fd);
    goto LAB_00016450;
  }
  local_40 = 3;
  local_3c = 8;
  local_38 = &local_34;
  iVar2 = ioctl((int)__fd,0x400c2602,&local_40);
  if (iVar2 < 0) {
    puts("get avparam node fail");
    local_24 = 0x1040003;
    iVar2 = ioctl((int)__fd,0x80042600,&local_24);
    if (-1 < iVar2) {
      local_38 = &local_2c;
      local_40 = 3;
      local_3c = 8;
      iVar2 = ioctl((int)__fd,0x800c2603,&local_40);
      if (-1 < iVar2) {
        close((int)__fd);
        goto LAB_00016450;
      }
      goto LAB_000165f8;
    }
    pcVar5 = puts;
    pcVar4 = "create avparam failed";
  }
  else {
    local_30._0_2_ = (undefined2)local_28;
    local_34 = CONCAT22(local_2c._2_2_,CONCAT11(local_2c._1_1_,(undefined1)local_34));
    local_38 = &local_34;
    iVar2 = ioctl((int)__fd,0x800c2603,&local_40);
    if (iVar2 < 0) {
LAB_000165f8:
      pcVar5 = puts;
      pcVar4 = "put avpram failed";
    }
    else {
      pcVar5 = close;
      pcVar4 = __fd;
    }
  }
  (*pcVar5)(pcVar4);
  close((int)__fd);
LAB_00016450:
  iVar2 = open64("/dev/persistentmem",2);
  if (iVar2 < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",iVar2);
  }
  else {
    local_48 = &g_avparam;
    local_50[0] = '\x03';
    local_50[1] = '\0';
    local_50[2] = '\0';
    local_50[3] = '\0';
    local_4c = CONCAT22(local_4c._2_2_,8);
    iVar3 = ioctl(iVar2,0x400c2602,local_50);
    if (iVar3 < 0) {
      puts("get avparam node fail");
      close(iVar2);
    }
    else {
      close(iVar2);
      if (DAT_00033655 != '\0') {
        DAT_00033656 = 0;
      }
      g_avparam_available = 1;
    }
  }
  return;
}



/* ===== FUNCTION get_avparam_volume @ 0001665c ===== */

undefined4 get_avparam_volume(undefined1 *param_1)

{
  undefined4 uVar1;
  
  if (g_avparam_inited == '\0') {
    fbdev_get_avparam();
    g_avparam_inited = '\x01';
  }
  if (g_avparam_available == '\0') {
    puts("avparam not available\r");
    uVar1 = 0xffffffff;
  }
  else {
    *param_1 = g_avparam;
    printf("volume is %d\r\n");
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION fbdev_setrotate @ 00016700 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void fbdev_setrotate(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  if (g_avparam_inited == '\0') {
    fbdev_get_avparam();
    g_avparam_inited = '\x01';
  }
  uVar3 = param_1 & 3;
  uVar1 = (param_1 << 0x1d) >> 0x1f;
  ge_rotate_flip_inited = 1;
  uVar2 = (param_1 << 0x1c) >> 0x1f;
  if ((param_1 & 0xc) == 0) {
    uVar1 = uVar1 ^ DAT_00033658;
    uVar2 = uVar2 ^ DAT_00033659;
    _DAT_00033658 = CONCAT11((char)uVar2,(char)uVar1);
    if (g_avparam_available == '\0') goto LAB_00016784;
  }
  else {
    g_avparam_available = '\x01';
    uVar1 = uVar1 ^ DAT_00033658;
    uVar2 = uVar2 ^ DAT_00033659;
    _DAT_00033658 = CONCAT11((char)uVar2,(char)uVar1);
  }
  if (DAT_00033656 == 0) {
    if (_DAT_00033658 == 0) {
LAB_00016784:
      if (uVar3 == 0) {
        ge_rotate_degree = 0;
      }
      else if (uVar3 == 1) {
        ge_rotate_degree = 0x5a;
      }
      else if (uVar3 == 2) {
        ge_rotate_degree = 0xb4;
      }
      else {
        ge_rotate_degree = 0x10e;
      }
      printf("game rotate:%d\n");
      printf("init rotate:%d\n",(uint)DAT_00033656);
      printf("init ver_flip:%d\n",(uint)DAT_00033659);
      printf("init hor_flip:%d\n",(uint)DAT_00033658);
      printf("rotate:%d\n",ge_rotate_degree);
      printf("ver_flip:%d\n",(uint)ge_ver_flip);
                    /* WARNING: Could not recover jumptable at 0x0001684c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      printf("hor_flip:%d\n",(uint)ge_hor_flip);
      return;
    }
    if (uVar3 == 0) {
      ge_rotate_degree = 0;
      goto LAB_0001689c;
    }
    if (uVar3 == 1) {
      ge_rotate_degree = 0x5a;
      goto LAB_0001689c;
    }
LAB_000169a4:
    iVar4 = 0xb4;
    if (uVar3 != 2) {
      iVar4 = 0x10e;
    }
  }
  else {
    iVar4 = 0;
    if (uVar3 != 0) {
      if (uVar3 != 1) goto LAB_000169a4;
      iVar4 = 0x5a;
    }
  }
  ge_rotate_degree = (uint)DAT_00033656 + iVar4;
  if (0x167 < ge_rotate_degree) {
    ge_rotate_degree = ge_rotate_degree - 0x168;
  }
  if (ge_rotate_degree == 0xb4) {
    ge_rotate_degree = 0;
    uVar1 = (uint)(uVar1 == 0);
    uVar2 = (uint)(uVar2 == 0);
  }
LAB_0001689c:
  ge_ver_flip = uVar2 != 0;
  ge_hor_flip = uVar1 != 0;
  printf("game rotate:%d\n");
  printf("init rotate:%d\n",(uint)DAT_00033656);
  printf("init ver_flip:%d\n",(uint)DAT_00033659);
  printf("init hor_flip:%d\n",(uint)DAT_00033658);
  printf("rotate:%d\n",ge_rotate_degree);
  printf("ver_flip:%d\n",(uint)ge_ver_flip);
                    /* WARNING: Could not recover jumptable at 0x0001696c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  printf("hor_flip:%d\n",(uint)ge_hor_flip);
  return;
}



/* ===== FUNCTION fbdev_init @ 00016a34 ===== */

undefined4 fbdev_init(int param_1,int param_2,uint param_3)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int *__arg;
  void *pvVar5;
  int *piVar6;
  void *pvVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  size_t __n;
  short sVar11;
  uint uVar12;
  pthread_attr_t pStack_70;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  ushort local_40;
  ushort local_3e;
  ushort local_3c;
  ushort local_3a;
  undefined1 auStack_38 [8];
  int local_30;
  
  local_40 = (ushort)param_1;
  local_3c = 0x140;
  local_4c = 0;
  local_48 = 0;
  local_44 = 0;
  local_3e = (ushort)param_2;
  local_3a = 0xf0;
  printf("color_format is %d\n",param_3);
  if (ge_rotate_flip_inited == '\0') {
    fbdev_setrotate(ge_rotate_degree);
    ge_rotate_flip_inited = '\x01';
  }
  iVar2 = open64("/dev/dis",2);
  if (iVar2 < 0) {
    return 0;
  }
  local_4c = 1;
  iVar3 = ioctl(iVar2,0xc00c0e0c,&local_4c);
  if (iVar3 != 0) {
    close(iVar2);
    puts("open /dev/dis failed");
    return 0;
  }
  close(iVar2);
  local_3c = (ushort)local_44;
  local_3a = local_44._2_2_;
  cube_msg_pthread_init();
  if (g_render != (int *)0x0) {
    fbdev_deinit();
  }
  if (param_3 < 6) {
    uVar4 = 1 << (param_3 & 0x1f);
    if ((uVar4 & 0x23) == 0) {
      if ((uVar4 & 0x14) == 0) {
        if ((uVar4 & 8) == 0) goto LAB_00016b8c;
        local_30 = 0x20;
      }
      else {
        local_30 = 0x18;
      }
    }
    else {
      local_30 = 0x10;
    }
  }
  else {
LAB_00016b8c:
    printf("%s:%d,Not support color format %d.\n","fbdev_init",700,param_3);
    local_30 = 0x10;
  }
  iVar2 = sunxi_gpio_init();
  if (iVar2 == 0) {
    puts("Init gpio_lib success.");
  }
  else {
    puts("Init gpio_lib error.");
  }
  __arg = calloc(0xee8,1);
  __arg[0x40] = param_1;
  __arg[0x41] = param_2;
  __arg[0x42] = local_44 & 0xffff;
  __arg[0x43] = local_44 >> 0x10;
  __arg[0x3b8] = param_3;
  iVar2 = hcge_open(__arg + 0x44);
  if (iVar2 != 0) {
    puts("Init hcge error.");
    goto LAB_0001728c;
  }
  iVar2 = open64("/dev/fb0",2);
  *__arg = iVar2;
  if (iVar2 < 0) {
    puts("Unable to open fb0");
    goto LAB_0001728c;
  }
  ioctl(iVar2,0x4611,1);
  piVar6 = __arg + 0x17;
  ioctl(*__arg,0x4602,__arg + 6);
  ioctl(*__arg,0x4600,piVar6);
  pvVar5 = mmap64((void *)0x0,__arg[0xb],3,1,*__arg,0);
  __arg[5] = (int)pvVar5;
  if (pvVar5 == (void *)0xffffffff) {
    puts("can\'t mmap");
    goto LAB_0001728c;
  }
  memset(pvVar5,0,__arg[0xb]);
  iVar2 = ge_rotate_degree;
  __arg[0x17] = param_1;
  __arg[0x18] = param_2;
  __arg[2] = iVar2;
  __arg[0x19] = param_1;
  __arg[0x1b] = 0;
  uVar1 = ge_hor_flip;
  __arg[0x1c] = 0;
  *(undefined1 *)((int)__arg + 0xd) = uVar1;
  __arg[0x2c] = 0;
  *(undefined1 *)(__arg + 3) = ge_ver_flip;
  __arg[4] = ge_is_full_screen;
  switch(param_3) {
  case 0:
  case 5:
    __arg[0x20] = 5;
    __arg[0x23] = 5;
    __arg[0x26] = 5;
    __arg[0x1d] = 0x10;
    iVar2 = 2;
    __arg[0x29] = 1;
    break;
  case 1:
    __arg[0x29] = 0;
    __arg[0x1d] = 0x10;
    iVar2 = 2;
    __arg[0x20] = 5;
    __arg[0x23] = 6;
    __arg[0x26] = 5;
    break;
  case 2:
  case 3:
  case 4:
    iVar2 = 4;
    __arg[0x20] = 8;
    __arg[0x1d] = 0x20;
    __arg[0x23] = 8;
    __arg[0x26] = 8;
    __arg[0x29] = 8;
    break;
  default:
    printf("not support bits_per_pixel %d\n",local_30);
    goto LAB_0001728c;
  }
  __arg[0x3b7] = iVar2;
  iVar3 = local_30 >> 3;
  uVar4 = param_1 * param_2 * iVar2;
  __arg[0x3b6] = iVar3;
  if (uVar4 == 0) {
    trap(7);
  }
  uVar12 = (uint)__arg[0xb] / uVar4;
  __arg[0x3f] = uVar4;
  if ((int)uVar12 < 2) {
LAB_0001738c:
    puts("frame buffer too less!");
  }
  else {
    if (uVar12 == 2) {
      __arg[0x45] = 2;
      __arg[0x370] = 0;
      __arg[0x1a] = param_2 << 1;
      __arg[0x378] = 0;
    }
    else {
      if (__arg[2] == 0) {
        iVar2 = uVar12 - 1;
        __arg[0x45] = iVar2;
      }
      else {
        iVar2 = uVar12 - 2;
        __arg[0x45] = iVar2;
      }
      if (iVar3 == 3) {
        sVar11 = (short)__arg[3];
        __arg[0x45] = iVar2 + -1;
LAB_00016eac:
        iVar2 = iVar2 + -1;
      }
      else {
        sVar11 = (short)__arg[3];
        if (sVar11 != 0) {
          __arg[0x45] = iVar2 + -1;
          goto LAB_00016eac;
        }
      }
      if (iVar2 < 2) goto LAB_0001738c;
      if (100 < iVar2) {
        iVar2 = 100;
        __arg[0x45] = 100;
      }
      iVar10 = __arg[5];
      __arg[0x1a] = param_2 * iVar2;
      __arg[0x370] = uVar4 * iVar2 + iVar10;
      if (__arg[2] == 0) {
        iVar9 = 1;
      }
      else {
        iVar9 = 2;
        __arg[0x378] = (iVar2 + 1) * uVar4 + iVar10;
      }
      iVar8 = iVar9 + iVar2;
      if (iVar3 == 3) {
        iVar9 = iVar9 + 1;
        __arg[0x380] = iVar8 * uVar4 + iVar10;
      }
      if (sVar11 != 0) {
        if (__arg[0x380] == 0) {
          __arg[0x388] = (iVar9 + iVar2) * uVar4 + iVar10;
        }
        else {
          __arg[0x388] = __arg[0x380];
        }
      }
    }
    iVar2 = ioctl(*__arg,0x4601,piVar6);
    if (iVar2 != -1) {
      printf("Initialize fb, fbnum %d, width %d, height %d, bits_per_pixel %d\n",__arg[0x45],
             __arg[0x17],__arg[0x18],local_30);
      ioctl(*__arg,0x4606,piVar6);
      ioctl(*__arg,0x20000d04,1);
      iVar2 = __arg[0x45];
      if (iVar2 < 1) {
        pvVar5 = (void *)__arg[5];
        __n = __arg[0x3f];
      }
      else {
        pvVar5 = (void *)__arg[5];
        __n = __arg[0x3f];
        piVar6 = __arg + 0x50;
        pvVar7 = pvVar5;
        do {
          *piVar6 = (int)pvVar7;
          piVar6 = piVar6 + 8;
          pvVar7 = (void *)((int)pvVar7 + __n);
        } while (__arg + (iVar2 + 10) * 8 != piVar6);
      }
      __arg[0x46] = -1;
      memset(pvVar5,0,__n);
      printf("w: %d\r\n",param_1);
      printf("h: %d\r\n",param_2);
      printf("screen w: %d\r\n",local_44 & 0xffff);
      printf("screen h: %d\r\n",local_44 >> 0x10);
      printf("scale.h_div = %d\r\n",(uint)local_40);
      printf("scale.v_div = %d\r\n",(uint)local_3e);
      printf("scale.h_mul = %d\r\n",(uint)local_3c);
      printf("scale.v_mul = %d\r\n",(uint)local_3a);
      printf("rotate:%d\n",ge_rotate_degree);
      ioctl(*__arg,0x80080d00,&local_40);
      ioctl(*__arg,0x80044620,auStack_38);
      ioctl(*__arg,0x4611,0);
      pthread_mutex_init((pthread_mutex_t *)(__arg + 0x389),(pthread_mutexattr_t *)0x0);
      pthread_cond_init((pthread_cond_t *)(__arg + 0x390),(pthread_condattr_t *)0x0);
      pthread_cond_init((pthread_cond_t *)(__arg + 0x39c),(pthread_condattr_t *)0x0);
      pthread_cond_init((pthread_cond_t *)(__arg + 0x3a8),(pthread_condattr_t *)0x0);
      pthread_attr_init(&pStack_70);
      pthread_attr_setstacksize(&pStack_70,0x2000);
      pthread_create((pthread_t *)(__arg + 0x3b4),&pStack_70,(__start_routine *)0x4fd0,__arg);
      pthread_create((pthread_t *)(__arg + 0x3b5),&pStack_70,(__start_routine *)0x52d8,__arg);
      g_render = __arg;
      return 0;
    }
    perror("Error reading variable information");
  }
LAB_0001728c:
  __fbdev_deinit(__arg);
  return 0xffffffff;
}



/* ===== FUNCTION fbdev_setfilp @ 000173b8 ===== */

void fbdev_setfilp(uint param_1,int param_2,byte param_3)

{
  ge_hor_flip = param_2 != 0;
  DAT_00033656 = (undefined2)param_1;
  ge_rotate_flip_inited = 1;
  DAT_00033659 = (byte)param_2;
  g_avparam_available = 1;
  ge_ver_flip = ge_hor_flip;
  ge_rotate_degree = param_1;
  DAT_00033658 = param_3;
  printf("init rotate:%d\n",param_1 & 0xffff);
  printf("init ver_flip:%d\n",(uint)DAT_00033659);
  printf("init hor_flip:%d\n",(uint)DAT_00033658);
  printf("rotate:%d\n",ge_rotate_degree);
  printf("ver_flip:%d\n",(uint)ge_ver_flip);
                    /* WARNING: Could not recover jumptable at 0x000174c8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  printf("hor_flip:%d\n",(uint)ge_hor_flip);
  return;
}



/* ===== FUNCTION fbdev_set_enhance @ 000174d0 ===== */

void fbdev_set_enhance(uint param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  uint local_38;
  uint local_34;
  uint local_30;
  uint local_2c;
  uint local_28;
  undefined4 local_24;
  
  if (g_render != (undefined4 *)0x0) {
    local_38 = 0;
    local_34 = 0;
    local_30 = 0;
    local_2c = 0;
    local_28 = 0;
    local_24 = 0;
    ioctl(*g_render,0x40180d02,&local_38);
    if (param_1 < 0x65) {
      local_38 = param_1;
    }
    if (param_2 < 0x65) {
      local_34 = param_2;
    }
    if (param_3 < 0x65) {
      local_30 = param_3;
    }
    if (param_4 < 0x65) {
      local_2c = param_4;
    }
    if (param_5 < 0xb) {
      local_28 = param_5;
    }
    ioctl(*g_render,0x80180d01,&local_38);
    puts("set enhance susscess\r");
    return;
  }
  puts("set enhance failed\r");
  return;
}



/* ===== FUNCTION fbdev_get_color_format_src @ 00017634 ===== */

undefined4 fbdev_get_color_format_src(void)

{
  if (g_render != 0) {
    return *(undefined4 *)(g_render + 0xee0);
  }
  return 3;
}



/* ===== FUNCTION gpio_init @ 00017660 ===== */

undefined4 gpio_init(void)

{
  int __fd;
  undefined4 uVar1;
  
  uVar1 = 0;
  if (reg_base == 0) {
    __fd = open64("/dev/mem",2);
    if (__fd < 0) {
      uVar1 = 0xfffffffb;
    }
    else {
      reg_base = (int)mmap64((void *)0x0,0x2020,3,1,__fd,0x18800000);
      if ((void *)reg_base == (void *)0xffffffff) {
        reg_base = 0;
        uVar1 = 0xffffffff;
      }
      else {
        ctrlreg = reg_base + 0x44;
        DAT_00033664 = reg_base + 0xc4;
        DAT_00033668 = reg_base + 0xe4;
        DAT_0003366c = reg_base + 0x344;
        pinmux_init(reg_base,0x18800000);
        uVar1 = 0;
      }
    }
  }
  return uVar1;
}



/* ===== FUNCTION gpio_deinit @ 00017754 ===== */

void gpio_deinit(void)

{
  if (reg_base != (void *)0x0) {
    munmap(reg_base,0x2020);
    reg_base = (void *)0x0;
  }
  pinmuxreg = 0;
  DAT_00033688 = 0;
  DAT_0003368c = 0;
  DAT_00033690 = 0;
  driver_cap = 0;
  return;
}



/* ===== FUNCTION gpio_set_output @ 000177a4 ===== */

void gpio_set_output(uint param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  
  if (param_1 < 0x80) {
    uVar2 = 1 << (param_1 & 0x1f);
    if (param_2 == 0) {
      iVar1 = (&ctrlreg)[param_1 >> 5];
      *(uint *)(iVar1 + 0x10) = ~uVar2 & *(uint *)(iVar1 + 0x10);
      return;
    }
    iVar1 = (&ctrlreg)[param_1 >> 5];
    *(uint *)(iVar1 + 0x10) = uVar2 | *(uint *)(iVar1 + 0x10);
  }
  return;
}



/* ===== FUNCTION gpio_get_input @ 00017810 ===== */

uint gpio_get_input(uint param_1)

{
  if (param_1 < 0x80) {
    return *(uint *)((&ctrlreg)[param_1 >> 5] + 0xc) >> (param_1 & 0x1f) & 1;
  }
  return 0xffffffff;
}



/* ===== FUNCTION gpio_configure @ 00017858 ===== */

undefined4 gpio_configure(uint param_1,uint param_2)

{
  if (0x7f < param_1) {
    return 0xffffffff;
  }
  pinmux_configure(param_1,0);
  if ((param_2 & 0xf) != 0) {
    if ((param_2 & 0xf) != 1) {
      return 0;
    }
    *(uint *)((&ctrlreg)[param_1 >> 5] + 0x14) =
         1 << (param_1 & 0x1f) | *(uint *)((&ctrlreg)[param_1 >> 5] + 0x14);
    return 0;
  }
  *(uint *)((&ctrlreg)[param_1 >> 5] + 0x14) =
       ~(1 << (param_1 & 0x1f)) & *(uint *)((&ctrlreg)[param_1 >> 5] + 0x14);
  return 0;
}



/* ===== FUNCTION gpio_get_configure @ 00017954 ===== */

uint gpio_get_configure(uint param_1)

{
  if (param_1 < 0x80) {
    return *(uint *)((&ctrlreg)[param_1 >> 5] + 0x14) >> (1 << (param_1 & 0x1f) & 0x1fU) & 1;
  }
  return 0xffffffff;
}



/* ===== FUNCTION pinmux_init @ 000179b0 ===== */

void pinmux_init(int param_1,int param_2)

{
  DAT_00033688 = param_1 + (0x188004e0 - param_2);
  DAT_0003368c = param_1 + (0x18800520 - param_2);
  DAT_00033690 = param_1 + (0x18800560 - param_2);
  pinmuxreg = param_1 + (0x188004a0 - param_2);
  driver_cap = param_1 + (0x18800184 - param_2);
  return;
}



/* ===== FUNCTION pinmux_deinit @ 00017a20 ===== */

void pinmux_deinit(void)

{
  pinmuxreg = 0;
  DAT_00033688 = 0;
  DAT_0003368c = 0;
  DAT_00033690 = 0;
  driver_cap = 0;
  return;
}



/* ===== FUNCTION pinmux_configure @ 00017a50 ===== */

undefined4 pinmux_configure(uint param_1,int param_2)

{
  if (param_1 - 0x60 < 6) {
    if (param_2 == 0) {
      param_2 = 0x38;
      *driver_cap = *driver_cap | 0x1000000;
    }
  }
  else {
    if (param_1 < 0x20) {
      *(char *)(pinmuxreg + param_1) = (char)param_2;
      return 0;
    }
    if (param_1 < 0x40) {
      *(char *)(DAT_00033688 + -0x20 + param_1) = (char)param_2;
      return 0;
    }
  }
  if (param_1 < 0x60) {
    *(char *)(DAT_0003368c + -0x40 + param_1) = (char)param_2;
    return 0;
  }
  if (param_1 < 0x80) {
    *(char *)(DAT_00033690 + -0x60 + param_1) = (char)param_2;
    return 0;
  }
  return 0xffffffff;
}



/* ===== FUNCTION sunxi_gpio_init @ 00017b40 ===== */

undefined4 sunxi_gpio_init(void)

{
  gpio_init();
  gpio_configure(9,1);
  gpio_configure(8,1);
  sunxi_gpio_inited = 1;
  return 0;
}



/* ===== FUNCTION sunxi_gpio_set_cfgpin @ 00017bac ===== */

undefined4 sunxi_gpio_set_cfgpin(undefined4 param_1,int param_2)

{
  if (sunxi_gpio_inited != 0) {
    if (param_2 != 0) {
      gpio_configure(param_1,1);
      return 0;
    }
    gpio_configure();
  }
  return 0;
}



/* ===== FUNCTION sunxi_gpio_get_cfgpin @ 00017c0c ===== */

bool sunxi_gpio_get_cfgpin(void)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = false;
  if (sunxi_gpio_inited != 0) {
    iVar2 = gpio_get_configure();
    bVar1 = iVar2 != 0;
  }
  return bVar1;
}



/* ===== FUNCTION sunxi_gpio_output @ 00017c50 ===== */

undefined4 sunxi_gpio_output(undefined4 param_1,int param_2)

{
  if (sunxi_gpio_inited != 0) {
    if (param_2 != 0) {
      gpio_set_output(param_1,1);
      return 0;
    }
    gpio_set_output();
  }
  return 0;
}



/* ===== FUNCTION sunxi_gpio_input @ 00017cb0 ===== */

bool sunxi_gpio_input(void)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = false;
  if (sunxi_gpio_inited != 0) {
    iVar2 = gpio_get_input();
    bVar1 = iVar2 != 0;
  }
  return bVar1;
}



/* ===== FUNCTION sunxi_gpio_cleanup @ 00017cf4 ===== */

void sunxi_gpio_cleanup(void)

{
  return;
}



/* ===== FUNCTION api_message_create_path @ 00017d00 ===== */

int api_message_create_path(char *param_1)

{
  FILE *__stream;
  key_t __key;
  int iVar1;
  char *__s;
  
  pthread_mutex_lock((pthread_mutex_t *)m_msg_mutex);
  __stream = fopen64(param_1,"w");
  if (__stream == (FILE *)0x0) {
    __s = "Error creating file";
  }
  else {
    fclose(__stream);
    __key = ftok(param_1,0x6b);
    if (__key != -1) {
      iVar1 = msgget(__key,0x3b6);
      if (iVar1 != -1) {
        pthread_mutex_unlock((pthread_mutex_t *)m_msg_mutex);
        printf("create msg id: %d\n",iVar1);
        return iVar1;
      }
      perror("msgget failed");
      pthread_mutex_unlock((pthread_mutex_t *)m_msg_mutex);
      return -1;
    }
    __s = "ftok";
  }
  perror(__s);
  pthread_mutex_unlock((pthread_mutex_t *)m_msg_mutex);
                    /* WARNING: Subroutine does not return */
  exit(1);
}



/* ===== FUNCTION api_message_create @ 00017e60 ===== */

int api_message_create(void)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)m_msg_mutex);
  iVar1 = msgget(0,0x3b6);
  if (iVar1 < 1) {
    perror("msgget failed");
    iVar1 = msgget(0,0x3b6);
    if (iVar1 < 1) {
      perror("msgget failed again");
      pthread_mutex_unlock((pthread_mutex_t *)m_msg_mutex);
      return -1;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)m_msg_mutex);
  printf("create msg id: %d\n",iVar1);
  return iVar1;
}



/* ===== FUNCTION api_message_delete @ 00017f44 ===== */

undefined4 api_message_delete(int param_1)

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



/* ===== FUNCTION api_message_send @ 00017fb0 ===== */

int api_message_send(int param_1,undefined4 param_2,size_t param_3)

{
  undefined4 local_a0;
  undefined1 auStack_9c [132];
  int local_18;
  
  if ((int)param_3 < 0x7f) {
    local_a0 = 1;
    __memcpy_chk(auStack_9c,param_2,param_3,0x80);
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



/* ===== FUNCTION api_message_receive @ 0001806c ===== */

undefined4 api_message_receive(int param_1,void *param_2,size_t param_3)

{
  ssize_t sVar1;
  undefined4 uVar2;
  undefined1 auStack_98 [4];
  undefined1 auStack_94 [136];
  
  sVar1 = msgrcv(param_1,auStack_98,param_3,0,0x800);
  if (sVar1 == -1) {
    uVar2 = 0xffffffff;
  }
  else {
    memcpy(param_2,auStack_94,param_3);
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION api_message_receive_block @ 000180ec ===== */

undefined4 api_message_receive_block(int param_1,void *param_2,size_t param_3)

{
  ssize_t sVar1;
  undefined4 uVar2;
  undefined1 auStack_98 [4];
  undefined1 auStack_94 [136];
  
  sVar1 = msgrcv(param_1,auStack_98,param_3,0,0);
  if (sVar1 == -1) {
    uVar2 = 0xffffffff;
  }
  else {
    memcpy(param_2,auStack_94,param_3);
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION api_message_receive_tm @ 00018168 ===== */

int api_message_receive_tm(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_4 == -1) {
    iVar2 = api_message_receive_block();
    return -(uint)(iVar2 != 0);
  }
  iVar2 = 0;
  if (0 < param_4) {
    do {
      iVar1 = api_message_receive(param_1,param_2,param_3);
      iVar2 = iVar2 + 1;
      if (iVar1 == 0) {
        return 0;
      }
      usleep(1000);
    } while (param_4 != iVar2);
  }
  return -1;
}



/* ===== FUNCTION api_message_get_count @ 0001826c ===== */

msgqnum_t api_message_get_count(int param_1)

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



/* ===== FUNCTION pcm_decoder_init @ 000182e0 ===== */

void * pcm_decoder_init(undefined1 param_1,undefined1 param_2,undefined4 param_3)

{
  void *__s;
  int iVar1;
  char *pcVar2;
  code *pcVar3;
  undefined4 local_20 [2];
  
  __s = malloc(0x280);
  local_20[0] = 0;
  memset(__s,0,0x278);
  *(undefined1 *)((int)__s + 2) = param_1;
  *(undefined4 *)((int)__s + 4) = 0x10000;
  *(undefined1 *)((int)__s + 3) = param_2;
  *(undefined4 *)((int)__s + 0xc) = param_3;
  *(undefined4 *)((int)__s + 0x18) = 1;
  *(undefined4 *)((int)__s + 0x1c) = 100;
  *(undefined4 *)((int)__s + 0x240) = 0;
  *(undefined4 *)((int)__s + 0x244) = 100;
  iVar1 = open64("/dev/auddec",2);
  *(int *)((int)__s + 0x278) = iVar1;
  printf("p->fd = %x\n",iVar1);
  if (*(int *)((int)__s + 0x278) < 0) {
    pcVar3 = puts;
    pcVar2 = "Open /dev/auddec error.";
LAB_000184a0:
    (*pcVar3)(pcVar2);
    free(__s);
    __s = (void *)0x0;
  }
  else {
    iVar1 = ioctl(*(int *)((int)__s + 0x278),0x82780301,__s);
    if (iVar1 != 0) {
      puts("Init auddec with I2SO & SPO error, try I2SO only...");
      *(undefined4 *)((int)__s + 0x18) = 1;
      iVar1 = ioctl(*(int *)((int)__s + 0x278),0x82780301,__s);
      if (iVar1 != 0) {
        puts("Init auddec with I2SO error.");
        pcVar2 = *(char **)((int)__s + 0x278);
        pcVar3 = close;
        goto LAB_000184a0;
      }
    }
    puts("AUDDEC_START");
    ioctl(*(int *)((int)__s + 0x278),0x20000304,0);
    ioctl(*(int *)((int)__s + 0x278),0x40041503,local_20);
    printf("audio buffer size is %d bytes\n",local_20[0]);
  }
  return __s;
}



/* ===== FUNCTION pcm_get_buffering_percent @ 000184d0 ===== */

void pcm_get_buffering_percent(undefined4 *param_1)

{
  int iVar1;
  undefined4 local_10 [2];
  
  local_10[0] = 0;
  if (-1 < *(int *)(hdl + 0x278)) {
    iVar1 = ioctl(*(int *)(hdl + 0x278),0x40041502,local_10);
    if (-1 < iVar1) {
      *param_1 = local_10[0];
    }
  }
  return;
}



/* ===== FUNCTION pcm_decode @ 00018538 ===== */

undefined4 pcm_decode(int param_1,void *param_2,uint param_3)

{
  ssize_t sVar1;
  uint uVar2;
  undefined4 local_28;
  undefined4 local_24;
  uint local_20;
  undefined4 local_1c;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  if (param_3 == 0) {
    puts("BUGS");
  }
  else {
    local_20 = param_3 & 0x3fffffff;
    local_28 = 0xffffffff;
    while (sVar1 = write(*(int *)(param_1 + 0x278),&local_28,0x10), sVar1 != 0x10) {
      puts("wait0");
      usleep(20000);
    }
    while (uVar2 = write(*(int *)(param_1 + 0x278),param_2,param_3), param_3 != uVar2) {
      puts("wait1");
      usleep(20000);
    }
  }
  return 0;
}



/* ===== FUNCTION pcm_decoder_flush @ 0001866c ===== */

void pcm_decoder_flush(int param_1)

{
  (*(code *)(undefined *)0x0)(*(undefined4 *)(param_1 + 0x278),0x20000305,0);
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
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION pcm_decoder_destroy @ 00018690 ===== */

void pcm_decoder_destroy(int param_1)

{
  if (param_1 == 0) {
    return;
  }
  if (0 < *(int *)(param_1 + 0x278)) {
    close(*(int *)(param_1 + 0x278));
  }
  (*(code *)(undefined *)0x0)(param_1);
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION pcm_decode_eos @ 000186f4 ===== */

void pcm_decode_eos(int param_1)

{
  ssize_t sVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18 [2];
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0x80000000;
  local_1c = 0;
  while (sVar1 = write(*(int *)(param_1 + 0x278),&local_28,0x10), sVar1 != 0x10) {
    usleep(20000);
  }
  while (ioctl(*(int *)(param_1 + 0x278),0x40040308,local_18), local_18[0] == 0) {
    usleep(50000);
  }
  return;
}



/* ===== FUNCTION pcm_play_task @ 000187c8 ===== */

undefined4 pcm_play_task(char *param_1)

{
  undefined4 uVar1;
  int __fd;
  void *__buf;
  int iVar2;
  ssize_t sVar3;
  
  printf("pcm_url = %s\n",param_1);
  uVar1 = pcm_decoder_init(0x10,2,0x2b11);
  printf("hdl = %x\n",uVar1);
  __fd = open64(param_1,0);
  if (__fd == 0) {
    puts("can not open alac url");
    uVar1 = 0xffffffff;
  }
  else {
    __buf = malloc(0x1000);
    if (__buf == (void *)0x0) {
      close(__fd);
      uVar1 = 0xffffffff;
    }
    else {
      do {
        sVar3 = read(__fd,__buf,0x1000);
        if (sVar3 != 0x1000) goto LAB_000188b8;
        iVar2 = pcm_decode(uVar1,__buf,0x1000);
      } while (iVar2 == 0);
      puts("pcm_decode error");
LAB_000188b8:
      pcm_decode_eos(uVar1);
      pcm_decoder_destroy(uVar1);
      close(__fd);
      free(__buf);
      uVar1 = 0;
    }
  }
  return uVar1;
}



/* ===== FUNCTION sound_driver_init @ 00018950 ===== */

void sound_driver_init(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  byte local_18;
  byte local_17 [11];
  
  printf("%s:%d\n","sound_driver_init",0xfb);
  hdl = pcm_decoder_init(0x10,param_3,param_2);
  iVar1 = get_avparam_volume(local_17);
  if (iVar1 == 0) {
    printf("set volume to %d\r\n",(uint)local_17[0]);
    local_18 = local_17[0];
    iVar1 = open64("/dev/sndC0i2so",1);
    if (-1 < iVar1) {
      ioctl(iVar1,0x8001080b,&local_18);
      ioctl(iVar1,0x2000080f,0);
      close(iVar1);
      return;
    }
    printf("open device:%s failed\n","/dev/sndC0i2so");
  }
  else {
    puts("skip set volume\r");
  }
  return;
}



/* ===== FUNCTION sound_driver_deinit @ 00018aa4 ===== */

undefined4 sound_driver_deinit(void)

{
  printf("%s:%d\n","sound_driver_deinit",0x108);
  pcm_decode_eos(hdl);
  pcm_decoder_destroy(hdl);
  hdl = 0;
  return 0;
}



/* ===== FUNCTION sound_driver_playframe @ 00018b18 ===== */

int sound_driver_playframe(undefined4 param_1,int param_2)

{
  int iVar1;
  int local_18 [2];
  
  if (((hdl != 0) && (pcm_get_buffering_percent(local_18), local_18[0] < 0x5b)) &&
     (iVar1 = pcm_decode(hdl,param_1,param_2 << 2), iVar1 != 0)) {
    puts("pcm_decode error");
    return iVar1;
  }
  return 0;
}



/* ===== FUNCTION sound_driver_flush @ 00018bd8 ===== */

void sound_driver_flush(void)

{
  if (hdl == 0) {
    return;
  }
  (*(code *)(undefined *)0x0)(*(undefined4 *)(hdl + 0x278),0x20000305,0);
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
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION hcge_open @ 00018c10 ===== */

undefined4 hcge_open(undefined4 *param_1)

{
  bool bVar1;
  void *__ptr;
  int iVar2;
  void *pvVar3;
  char *pcVar4;
  int local_18;
  size_t local_14;
  
  local_18 = 0;
  local_14 = 0;
  __ptr = calloc(1,0x438);
  if (__ptr == (void *)0x0) {
    puts("Not enough memory");
    return 0xfffffff4;
  }
  iVar2 = open64("/dev/ge",2);
  *(int *)((int)__ptr + 0x108) = iVar2;
  if (iVar2 < 0) {
    printf("Open ge error.%s\n","/dev/ge");
  }
  else {
    pvVar3 = mmap64((void *)0x0,0x2000,3,1,iVar2,0);
    if (pvVar3 == (void *)0xffffffff) {
      printf("map ge reg error.");
    }
    else {
      *(void **)((int)__ptr + 0xe4) = pvVar3;
      ioctl(*(int *)((int)__ptr + 0x108),0x20002305,&local_18);
      pvVar3 = mmap64((void *)0x0,local_14,3,1,*(int *)((int)__ptr + 0x108),0x2000);
      if (pvVar3 != (void *)0xffffffff) {
        *(void **)((int)__ptr + 0xec) = pvVar3;
        *(int *)((int)__ptr + 0xe8) = local_18;
        *(int *)((int)__ptr + 0x100) = local_18 + 0x20;
        *(size_t *)((int)__ptr + 0xf0) = local_14;
        *(int *)((int)__ptr + 0xf4) = local_18 + 0x420;
        *(int *)((int)__ptr + 0xf8) = (int)pvVar3 + 0x420;
        *(size_t *)((int)__ptr + 0xfc) = local_14 - 0x420;
        *(undefined1 *)((int)__ptr + 0x10d) = 0;
        *(undefined1 *)((int)__ptr + 0x10f) = 0;
        *(undefined1 *)((int)__ptr + 0x110) = 0;
        *(undefined4 *)((int)__ptr + 0x118) = 0;
        *(undefined4 *)((int)__ptr + 0x11c) = 0;
        *(undefined1 *)((int)__ptr + 0x120) = 0;
        *(undefined1 *)((int)__ptr + 0x111) = 1;
        pcVar4 = getenv("HCHIP_ENABLE_LOG");
        if ((pcVar4 == (char *)0x0) || (bVar1 = true, *pcVar4 != 'Y')) {
          iVar2 = access("/tmp/hchip_enable_log",0);
          bVar1 = iVar2 == 0;
        }
        *(bool *)((int)__ptr + 0x10e) = bVar1;
        printf("******************%s:%d***********************\n","hcge_driver_init",0x28);
        hcge_reset(__ptr);
        *(undefined4 *)((int)__ptr + 0x104) = *(undefined4 *)(*(int *)((int)__ptr + 0xe4) + 0xac);
        ioctl(*(int *)((int)__ptr + 0x108),0x20002301,0);
        pvVar3 = calloc(0x2a8,1);
        *(void **)((int)__ptr + 0x158) = pvVar3;
        *param_1 = __ptr;
        return 0;
      }
      puts("mmap HW cmdQ buffer error");
    }
  }
  if (*(void **)((int)__ptr + 0xe4) != (void *)0x0) {
    munmap(*(void **)((int)__ptr + 0xe4),0x2000);
  }
  if (*(void **)((int)__ptr + 0xec) != (void *)0x0) {
    munmap(*(void **)((int)__ptr + 0xec),local_14);
  }
  if (0 < *(int *)((int)__ptr + 0x108)) {
    close(*(int *)((int)__ptr + 0x108));
  }
  free(__ptr);
  return 0xffffffff;
}



/* ===== FUNCTION hcge_close @ 00018f20 ===== */

void hcge_close(int param_1)

{
  if (param_1 == 0) {
    return;
  }
  ioctl(*(int *)(param_1 + 0x108),0x20002302,0);
  if (*(void **)(param_1 + 0xe4) != (void *)0x0) {
    munmap(*(void **)(param_1 + 0xe4),0x2000);
  }
  if (*(void **)(param_1 + 0xec) != (void *)0x0) {
    munmap(*(void **)(param_1 + 0xec),*(size_t *)(param_1 + 0xf0));
  }
  if (*(void **)(param_1 + 0x158) != (void *)0x0) {
    free(*(void **)(param_1 + 0x158));
  }
  if (0 < *(int *)(param_1 + 0x108)) {
    close(*(int *)(param_1 + 0x108));
  }
  (*(code *)(undefined *)0x0)(param_1);
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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION hcge_hw_reset @ 00018fe8 ===== */

void hcge_hw_reset(int param_1)

{
  uint *puVar1;
  int iVar2;
  
  if (*(int *)(*(int *)(param_1 + 0xe4) + 8) < 0) {
    iVar2 = 0x65;
    do {
      iVar2 = iVar2 + -1;
      puts("\n\n!!!!!!!!!!!WARNING!!!!!!!! goto reset ge, but ge busy,retry\n");
      usleep(5000);
      if (-1 < *(int *)(*(int *)(param_1 + 0xe4) + 8)) break;
    } while (iVar2 != 0);
  }
  ioctl(*(int *)(param_1 + 0x108),0x20002303,0);
  ioctl(*(int *)(param_1 + 0x108),0x20002306,3);
  puVar1 = *(uint **)(param_1 + 0xe4);
  puVar1[0x3c] = 0;
  *puVar1 = *puVar1 & 0xfffdffff | 0x20000;
  *puVar1 = *puVar1 & 0x7fffffff | 0x80000000;
  return;
}



/* ===== FUNCTION gen_fliter_coef @ 000190e0 ===== */

void gen_fliter_coef(undefined4 param_1,undefined4 param_2,uint *param_3)

{
  ushort uVar1;
  int iVar2;
  ushort *puVar3;
  int iVar4;
  ushort *__s;
  undefined1 auStack_228 [260];
  undefined1 auStack_124 [132];
  ushort local_a0;
  ushort local_9e;
  ushort local_9c;
  ushort local_9a;
  ushort local_98;
  ushort local_96;
  ushort local_94;
  ushort local_92;
  ushort local_90;
  ushort local_8e;
  ushort local_8c;
  ushort local_8a;
  ushort local_88;
  ushort local_86;
  ushort local_84;
  ushort local_82;
  ushort local_80;
  ushort local_7e;
  ushort local_7c;
  ushort local_7a;
  ushort local_78;
  ushort local_76;
  ushort local_74;
  ushort local_72;
  ushort local_70;
  ushort local_6e;
  ushort local_6c;
  ushort local_6a;
  ushort local_68;
  ushort local_66;
  ushort local_64;
  ushort local_62;
  ushort local_60;
  ushort local_5e;
  ushort local_5c;
  ushort local_5a;
  ushort local_58;
  ushort local_56;
  ushort local_54;
  ushort local_52;
  ushort local_50;
  ushort local_4e;
  ushort local_4c;
  ushort local_4a;
  ushort local_48;
  ushort local_46;
  ushort local_44;
  ushort local_42;
  ushort local_40;
  ushort local_3e;
  ushort local_3c;
  ushort local_3a;
  ushort local_38;
  ushort local_36;
  ushort local_34;
  ushort local_32;
  ushort local_30;
  ushort local_2e;
  ushort local_2c;
  ushort local_2a;
  ushort local_28;
  ushort local_26;
  ushort local_24;
  ushort local_22;
  
  memset(auStack_228,0,0x104);
  __s = &local_a0;
  memset(auStack_124,0,0x82);
  memset(__s,0,0x80);
  memset(param_3,0,0x40);
  iVar4 = 0;
  designfilterff(param_1,param_2,0x10,4,1,0x10000,8,auStack_228,auStack_124);
  do {
    iVar2 = 0;
    puVar3 = __s;
    do {
      uVar1 = extract_coef(auStack_124,0x10,4,iVar2,iVar4);
      iVar2 = iVar2 + 1;
      *puVar3 = uVar1;
      puVar3 = puVar3 + 1;
    } while (iVar2 != 0x10);
    iVar4 = iVar4 + 1;
    __s = __s + 0x10;
  } while (iVar4 != 4);
  param_3[1] = local_80 & 0x1ff;
  param_3[2] = local_60 & 0x1ff;
  param_3[3] = local_40 & 0x1ff;
  param_3[4] = local_9e & 0x1ff;
  param_3[5] = local_7e & 0x1ff;
  param_3[6] = local_5e & 0x1ff;
  param_3[7] = local_3e & 0x1ff;
  param_3[8] = local_9c & 0x1ff;
  param_3[9] = local_7c & 0x1ff;
  param_3[10] = local_5c & 0x1ff;
  param_3[0xb] = local_3c & 0x1ff;
  param_3[0xc] = local_9a & 0x1ff;
  param_3[0xd] = local_7a & 0x1ff;
  param_3[0xe] = local_5a & 0x1ff;
  param_3[0xf] = local_3a & 0x1ff;
  *param_3 = local_a0 & 0x1ff;
  param_3[0x11] = local_78 & 0x1ff;
  param_3[0x12] = local_58 & 0x1ff;
  param_3[0x13] = local_38 & 0x1ff;
  param_3[0x14] = local_96 & 0x1ff;
  param_3[0x15] = local_76 & 0x1ff;
  param_3[0x16] = local_56 & 0x1ff;
  param_3[0x17] = local_36 & 0x1ff;
  param_3[0x18] = local_94 & 0x1ff;
  param_3[0x19] = local_74 & 0x1ff;
  param_3[0x1a] = local_54 & 0x1ff;
  param_3[0x1b] = local_34 & 0x1ff;
  param_3[0x1c] = local_92 & 0x1ff;
  param_3[0x1d] = local_72 & 0x1ff;
  param_3[0x1e] = local_52 & 0x1ff;
  param_3[0x1f] = local_32 & 0x1ff;
  param_3[0x10] = local_98 & 0x1ff;
  param_3[0x21] = local_70 & 0x1ff;
  param_3[0x22] = local_50 & 0x1ff;
  param_3[0x23] = local_30 & 0x1ff;
  param_3[0x24] = local_8e & 0x1ff;
  param_3[0x25] = local_6e & 0x1ff;
  param_3[0x26] = local_4e & 0x1ff;
  param_3[0x27] = local_2e & 0x1ff;
  param_3[0x28] = local_8c & 0x1ff;
  param_3[0x29] = local_6c & 0x1ff;
  param_3[0x2a] = local_4c & 0x1ff;
  param_3[0x2b] = local_2c & 0x1ff;
  param_3[0x2c] = local_8a & 0x1ff;
  param_3[0x2d] = local_6a & 0x1ff;
  param_3[0x2e] = local_4a & 0x1ff;
  param_3[0x2f] = local_2a & 0x1ff;
  param_3[0x20] = local_90 & 0x1ff;
  param_3[0x30] = local_88 & 0x1ff;
  param_3[0x31] = local_68 & 0x1ff;
  param_3[0x32] = local_48 & 0x1ff;
  param_3[0x33] = local_28 & 0x1ff;
  param_3[0x34] = local_86 & 0x1ff;
  param_3[0x35] = local_66 & 0x1ff;
  param_3[0x36] = local_46 & 0x1ff;
  param_3[0x37] = local_26 & 0x1ff;
  param_3[0x38] = local_84 & 0x1ff;
  param_3[0x39] = local_64 & 0x1ff;
  param_3[0x3a] = local_44 & 0x1ff;
  param_3[0x3b] = local_24 & 0x1ff;
  param_3[0x3c] = local_82 & 0x1ff;
  param_3[0x3d] = local_62 & 0x1ff;
  param_3[0x3e] = local_42 & 0x1ff;
  param_3[0x3f] = local_22 & 0x1ff;
  return;
}



/* ===== FUNCTION hcge_engine_sync @ 00019538 ===== */

undefined4 hcge_engine_sync(int param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  piVar4 = *(int **)(param_1 + 0xec);
  iVar7 = *(int *)(param_1 + 0xe4);
  uVar1 = piVar4[6] & 0xfffffffe;
  uVar2 = *(uint *)(iVar7 + 0x10);
  if ((piVar4[6] & 1U) == 0) {
    uVar2 = uVar1 - uVar2;
  }
  else {
    if ((uint)piVar4[5] <= uVar2) {
      iVar5 = 0x50;
      iVar6 = iVar7;
      goto LAB_000195f4;
    }
    uVar2 = ((uVar1 - uVar2) + piVar4[5]) - *piVar4;
  }
  iVar5 = uVar2 / 0x50 + 0x50;
  iVar6 = iVar7;
LAB_000195f4:
  do {
    iVar8 = *(int *)(iVar6 + 0x10);
    iVar3 = ioctl(*(int *)(param_1 + 0x108),0x20002304,iVar5);
    iVar6 = *(int *)(param_1 + 0xe4);
    if (iVar3 == 0) {
      if (uVar1 == *(uint *)(iVar7 + 0x10)) {
        while (*(int *)(iVar6 + 8) < 0) {
          usleep(1000);
        }
      }
      else {
        if (*(char *)(param_1 + 0x10e) != '\0') {
          printf("%s() GE finish but fst_ptr != buf_end, now reset GE!\n","hcge_engine_sync");
        }
LAB_00019670:
        hcge_reset(param_1);
      }
      return 0;
    }
    if (iVar8 == *(int *)(iVar6 + 0x10)) {
      if ((uVar1 == *(uint *)(iVar7 + 0x10)) && (*(int *)(iVar6 + 8) < 0)) {
        return 0;
      }
      if (*(char *)(param_1 + 0x10e) != '\0') {
        printf("%s() %s() : GE Timeout with %lu! Now reset GE!\n","hcge_engine_sync",
               "hcge_engine_sync",iVar5);
      }
      goto LAB_00019670;
    }
    if (*(char *)(param_1 + 0x10e) != '\0') {
      printf("%s() %s() : GE Timeout, but GE cmd is not finish, retry! \n","hcge_engine_sync",
             "hcge_engine_sync");
      iVar6 = *(int *)(param_1 + 0xe4);
    }
  } while( true );
}



/* ===== FUNCTION hcge_draw_rect @ 00019744 ===== */

undefined4 hcge_draw_rect(int param_1,uint *param_2)

{
  ushort uVar1;
  ushort uVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int local_18 [3];
  
  puVar3 = *(uint **)(param_1 + 0x158);
  if (*(char *)(param_1 + 0x10d) == '\0') {
    uVar7 = *puVar3;
  }
  else {
    uVar6 = puVar3[0x17];
    uVar5 = puVar3[0x18];
    puVar3[1] = puVar3[1] & 0xefffffff | 0x10000000;
    uVar1 = *(ushort *)(param_1 + 0xcc);
    uVar7 = *puVar3 | 0x4000;
    uVar4 = uVar1 & 0xfff;
    puVar3[0x17] = uVar6 & 0xfffff000 | uVar4;
    uVar2 = *(ushort *)(param_1 + 0xd0);
    puVar3[0x17] = uVar6 & 0xf000f000 | uVar4 | (uVar2 & 0xfff) << 0x10;
    uVar4 = (*(int *)(param_1 + 0xd4) + 1) - (uint)uVar1 & 0xfff;
    puVar3[0x18] = uVar5 & 0xfffff000 | uVar4;
    puVar3[0x18] = uVar5 & 0xf000f000 | uVar4 |
                   ((*(int *)(param_1 + 0xd8) + 1) - (uint)uVar2 & 0xfff) << 0x10;
  }
  uVar5 = puVar3[5];
  uVar4 = puVar3[6];
  puVar3[1] = puVar3[1] & 0xfffff8ff | 0x100;
  uVar8 = *param_2 & 0xfff;
  puVar3[0x1e] = *(uint *)(param_1 + 0x114);
  puVar3[5] = uVar5 & 0xfffff000 | uVar8;
  uVar9 = param_2[1];
  puVar3[5] = uVar5 & 0xf000f000 | uVar8 | (uVar9 & 0xfff) << 0x10;
  uVar5 = param_2[2];
  puVar3[6] = uVar4 & 0xfffff000 | uVar5 & 0xfff;
  uVar6 = param_2[3];
  puVar3[10] = puVar3[10] & 0xf000f000 | uVar8 | (uVar9 & 0xfff) << 0x10;
  *puVar3 = uVar7 | 0x300;
  puVar3[6] = uVar4 & 0xf000f000 | uVar5 & 0xfff | (uVar6 & 0xfff) << 0x10;
  if (*(char *)(param_1 + 0x111) == '\0') {
    hcge_feed_nodes(param_1,matrix_disable_node_6155,&DAT_000332fc,"matrix_disable_node");
  }
  local_18[0] = param_1 + 0x15c;
  hcge_construct_nodes(param_1,local_18);
  hcge_feed_nodes(param_1,param_1 + 0x15c,local_18[0],"hcge_draw_rect");
  *(undefined1 *)(param_1 + 0x111) = 1;
  return 1;
}



/* ===== FUNCTION hcge_fill_rect @ 000198d4 ===== */

undefined4 hcge_fill_rect(int param_1,uint *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint *puVar9;
  int local_60 [2];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  uint local_48;
  uint local_44;
  uint local_40;
  uint local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  
  puVar9 = *(uint **)(param_1 + 0x158);
  if (*(char *)(param_1 + 0x10d) == '\0') {
    cVar1 = *(char *)(param_1 + 0x10e);
  }
  else {
    iVar3 = hcge_clip_rect();
    if (iVar3 == 0) {
      return 1;
    }
    cVar1 = *(char *)(param_1 + 0x10e);
  }
  if (cVar1 != '\0') {
    local_2c = *(undefined4 *)(param_1 + 0xb0);
    uVar7 = *param_2;
    uVar5 = param_2[1];
    uVar8 = param_2[2];
    uVar6 = param_2[3];
    local_58 = hcge_pixelformat_name(*(undefined4 *)(param_1 + 0x48));
    local_3c = (uint)*(byte *)(param_1 + 0x84);
    local_40 = (uint)*(byte *)(param_1 + 0x85);
    local_44 = (uint)*(byte *)(param_1 + 0x86);
    local_48 = (uint)*(byte *)(param_1 + 0x87);
    local_34 = *(undefined4 *)(param_1 + 0x4c);
    local_38 = *(undefined4 *)(param_1 + 0x50);
    local_30 = *(undefined4 *)(param_1 + 0xb4);
    uVar2 = puVar9[4];
    local_4c = hcge_drawingflags_to_string(*(undefined4 *)(param_1 + 4));
    local_50 = *(undefined4 *)(param_1 + 4);
    local_54 = hcge_blend_to_string(*(undefined4 *)(param_1 + 0xa0));
    uVar4 = hcge_blend_to_string(*(undefined4 *)(param_1 + 0xa4));
    printf("%s() --> 0x%08lx, @--> {(%d, %d) , (%d, %d)} - (%d x %d), %s, dpitch(%d - %d), size = (%d X %d), color = (%02x | %02x_%02x_%02x), \n\t\t\t\t\t\t\t dflags = %s(0x%08x), src_blend = %s, dst_blend = %s\n"
           ,"hcge_fill_rect",local_2c,uVar7,uVar5,uVar7 + uVar8 + -1,uVar5 + uVar6 + -1,uVar8,uVar6,
           local_58,local_30,uVar2 & 0xfff,local_34,local_38,local_3c,local_40,local_44,local_48,
           local_4c,local_50,local_54,uVar4);
  }
  uVar5 = puVar9[5];
  uVar8 = *(uint *)(param_1 + 0x114);
  uVar7 = *param_2 & 0xfff;
  uVar2 = puVar9[6];
  puVar9[5] = uVar5 & 0xfffff000 | uVar7;
  uVar6 = param_2[1];
  puVar9[0x1c] = uVar8;
  puVar9[5] = uVar5 & 0xf000f000 | uVar7 | (uVar6 & 0xfff) << 0x10;
  uVar5 = param_2[2];
  puVar9[6] = uVar2 & 0xfffff000 | uVar5 & 0xfff;
  uVar8 = param_2[3];
  puVar9[10] = puVar9[10] & 0xf000f000 | uVar7 | (uVar6 & 0xfff) << 0x10;
  *puVar9 = *puVar9 | 0x300;
  puVar9[6] = uVar2 & 0xf000f000 | uVar5 & 0xfff | (uVar8 & 0xfff) << 0x10;
  if (*(char *)(param_1 + 0x111) == '\0') {
    hcge_feed_nodes(param_1,matrix_disable_node_6155,&DAT_000332fc,"matrix_disable_node");
  }
  local_60[0] = param_1 + 0x15c;
  hcge_construct_nodes(param_1,local_60);
  hcge_feed_nodes(param_1,param_1 + 0x15c,local_60[0],"hcge_fill_rect");
  *(undefined1 *)(param_1 + 0x111) = 1;
  return 1;
}



/* ===== FUNCTION hcge_blit @ 00019bb4 ===== */

char hcge_blit(undefined8 param_1,int param_2,uint *param_3,int param_4,int param_5)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint *puVar14;
  byte *pbVar15;
  byte *pbVar16;
  uint uVar17;
  uint uVar18;
  uint *puVar19;
  uint *puVar20;
  code *pcVar21;
  double dVar22;
  undefined8 uVar23;
  uint local_f8;
  uint uStack_f4;
  uint local_f0;
  uint uStack_ec;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 local_d8;
  undefined8 local_d0;
  uint local_c8;
  uint local_c4;
  uint local_c0;
  uint local_bc;
  uint local_b8;
  uint local_b4;
  uint local_b0;
  uint local_ac;
  int local_a8;
  int local_a4;
  uint local_a0;
  uint local_9c;
  int local_98 [2];
  uint local_90;
  uint local_8c;
  uint local_88;
  undefined4 local_84;
  undefined4 local_80;
  uint local_7c;
  uint local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  uint local_68;
  uint local_64;
  uint local_60;
  uint local_5c;
  uint local_58;
  undefined4 local_54;
  undefined4 local_50;
  uint local_4c;
  uint local_48;
  undefined4 local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  uint local_30;
  
  uVar7 = (undefined4)((ulonglong)param_1 >> 0x20);
  if ((0x4000 < *(uint *)(param_2 + 0xbc)) || (0x4000 < *(uint *)(param_2 + 0xb4))) {
    return '\0';
  }
  cVar4 = *(char *)(param_2 + 0x120);
  if (cVar4 != '\0') {
    puVar20 = *(uint **)(param_2 + 0x158);
    hcge_get_inverse_matrix(param_2 + 0x128,&local_f8);
    uVar23 = CONCAT44(uVar7,(float)-param_4);
    hcge_matrix_translate_left_multiply(uVar23,(float)-param_5);
    uVar7 = (undefined4)((ulonglong)uVar23 >> 0x20);
    local_a0 = param_3[2];
    local_9c = param_3[3];
    local_a8 = param_4;
    local_a4 = param_5;
    iVar5 = hcge_get_bounding_rect(param_2,&local_a8,&local_b8);
    if (iVar5 == 0) {
      return cVar4;
    }
    if ((*(char *)(param_2 + 0x10d) != '\0') &&
       (iVar5 = hcge_clip_rect(param_2,&local_b8), iVar5 == 0)) {
      return cVar4;
    }
    iVar5 = hcge_clip_qw(&local_b8,&local_c8);
    if (iVar5 != 0) {
      uVar9 = puVar20[0x17];
      uVar6 = puVar20[0x18];
      puVar20[1] = puVar20[1] & 0xefffffff | 0x10000000;
      *puVar20 = *puVar20 | 0x4000;
      puVar20[0x17] = uVar9 & 0xfffff000 | local_c8 & 0xfff;
      puVar20[0x17] = uVar9 & 0xf000f000 | local_c8 & 0xfff | (local_c4 & 0xfff) << 0x10;
      puVar20[0x18] = uVar6 & 0xfffff000 | local_c0 & 0xfff;
      puVar20[0x18] = uVar6 & 0xf000f000 | local_c0 & 0xfff | (local_bc & 0xfff) << 0x10;
    }
    if (*(char *)(param_2 + 0x10e) != '\0') {
      local_30 = param_3[3];
      local_44 = *(undefined4 *)(param_2 + 0xb0);
      local_8c = *(uint *)(param_2 + 0xb8);
      local_34 = *param_3;
      local_38 = param_3[1];
      uVar6 = param_3[2];
      local_40 = local_ac;
      local_3c = local_b8;
      local_88 = local_b4;
      local_90 = local_b0;
      local_84 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x5c));
      local_80 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x48));
      local_70 = *(undefined4 *)(param_2 + 100);
      local_6c = *(undefined4 *)(param_2 + 0xb4);
      local_7c = *(uint *)(param_2 + 0xbc);
      local_5c = (uint)*(byte *)(param_2 + 0x84);
      local_64 = *(uint *)(param_2 + 0x4c);
      local_60 = *(uint *)(param_2 + 0x50);
      local_78 = puVar20[0xd] & 0xfff;
      local_74 = *(undefined4 *)(param_2 + 0x60);
      local_58 = (uint)*(byte *)(param_2 + 0x85);
      local_48 = (uint)*(byte *)(param_2 + 0x86);
      local_4c = (uint)*(byte *)(param_2 + 0x87);
      local_68 = puVar20[4] & 0xfff;
      local_50 = hcge_blittingflags_to_string(*(undefined4 *)(param_2 + 0xc));
      local_54 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa0));
      uVar8 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa4));
      printf("%s() 0x%08lx --> 0x%08lx, {(%d, %d) , (%d, %d)} - [%d X %d]@--> {(%d, %d) , (%d, %d)} - (%d x %d), \n\t\t\t\t\t      @[brect] {(%4d, %4d) , (%4d, %4d)} - [%4d X %4d]@, %s --> %s, spitch(%d - %d) size (%d, %d) -> dpitch(%d - %d) size(%d, %d), state->color = (%02x | %02x_%02x_%02x),\n\t\t\t\t\t       bflags = %s, src_blend = %s, dst_blend = %s&\n"
             ,"hcge_blit_matrix",local_8c,local_44,local_34,local_38,local_34 + uVar6 + -1,
             local_38 + local_30 + -1,uVar6,local_30,param_4,param_5,param_4 + uVar6 + -1,
             param_5 + local_30 + -1,uVar6,local_30,local_3c,local_88,local_3c + local_90 + -1,
             local_88 + local_40 + -1,local_90,local_40,local_84,local_80,local_7c,local_78,local_74
             ,local_70,local_6c,local_68,local_64,local_60,local_5c,local_58,local_48,local_4c,
             local_50,local_54,uVar8);
    }
    hcge_matrix_translate_right_multiply
              (CONCAT44(uVar7,(float)(int)((int)local_b0 / 2 + local_b8)),
               (float)(int)((int)local_ac / 2 + local_b4),local_b8,local_ac,&local_f8);
    puVar20[0xd] = puVar20[0xd] & 0xbfffffff | 0x40000000;
    puVar20[1] = puVar20[1] & 0xffffcfff | 0x3000;
    puVar20[0x23] = puVar20[0x23] & 0xffffff3f | 0x80;
    *puVar20 = *puVar20 | 0x40000;
    uVar6 = hcge_ge_coeff(CONCAT44(uStack_f4,local_f8));
    puVar20[0x24] = uVar6;
    uVar6 = hcge_ge_coeff(CONCAT44(uStack_ec,local_f0));
    puVar20[0x25] = uVar6;
    uVar6 = hcge_ge_coeff(local_e8);
    puVar20[0x26] = uVar6;
    uVar6 = hcge_ge_coeff(local_e0);
    puVar20[0x27] = uVar6;
    uVar6 = hcge_ge_coeff(local_d8);
    puVar20[0x28] = uVar6;
    uVar6 = hcge_ge_coeff(local_d0);
    uVar9 = *param_3;
    uVar13 = puVar20[0xe];
    uVar12 = puVar20[0xf];
    uVar11 = puVar20[5];
    puVar20[0xe] = uVar13 & 0xfffff000 | uVar9 & 0xfff;
    uVar17 = param_3[1];
    puVar20[0x29] = uVar6;
    uVar10 = puVar20[6];
    puVar20[0xe] = uVar13 & 0xf000f000 | uVar9 & 0xfff | (uVar17 & 0xfff) << 0x10;
    uVar6 = param_3[2];
    puVar20[0xf] = uVar12 & 0xfffff000 | uVar6 & 0xfff;
    puVar20[0xf] = uVar12 & 0xf000f000 | uVar6 & 0xfff | (param_3[3] & 0xfff) << 0x10;
    local_b8 = local_b8 & 0xfff;
    puVar20[5] = uVar11 & 0xfffff000 | local_b8;
    puVar20[5] = uVar11 & 0xf000f000 | local_b8 | (local_b4 & 0xfff) << 0x10;
    puVar20[6] = uVar10 & 0xfffff000 | local_b0 & 0xfff;
    puVar20[10] = puVar20[10] & 0xf000f000 | local_b8 | (local_b4 & 0xfff) << 0x10;
    *puVar20 = *puVar20 | 0x700;
    puVar20[6] = uVar10 & 0xf000f000 | local_b0 & 0xfff | (local_ac & 0xfff) << 0x10;
    if (*(char *)(param_2 + 0x111) != '\0') {
      matrix_fix_node_6149._8_4_ = puVar20[2] & 0xfffffff;
      hcge_feed_nodes(param_2,matrix_fix_node_6149,matrix_disable_node_6155,"HW fix node");
    }
    local_98[0] = param_2 + 0x15c;
    hcge_construct_nodes(param_2,local_98);
    hcge_feed_nodes(param_2,param_2 + 0x15c,local_98[0],"hcge_blit_matrix");
    *(undefined1 *)(param_2 + 0x111) = 0;
    return cVar4;
  }
  cVar4 = *(char *)(param_2 + 0x10d);
  local_b8 = param_4 + *(int *)(param_2 + 0x118);
  puVar20 = *(uint **)(param_2 + 0x158);
  local_c8 = param_5 + *(int *)(param_2 + 0x11c);
  if (*(double *)(param_2 + 0x428) == 0.0) {
    if ((cVar4 != '\0') && (iVar5 = hcge_clip_blit(param_2,param_3,&local_b8,&local_c8), iVar5 == 0)
       ) {
      return cVar4;
    }
  }
  else if (cVar4 != '\0') {
    local_f8 = *(uint *)(param_2 + 0xcc);
    uStack_f4 = *(uint *)(param_2 + 0xd0);
    local_f0 = (*(int *)(param_2 + 0xd4) - local_f8) + 1;
    uStack_ec = (*(int *)(param_2 + 0xd8) - uStack_f4) + 1;
    iVar5 = hcge_clip_rect(param_2,&local_f8);
    if (iVar5 == 0) {
      return cVar4;
    }
    uVar9 = puVar20[0x17];
    uVar6 = puVar20[0x18];
    puVar20[1] = puVar20[1] & 0xefffffff | 0x10000000;
    *puVar20 = *puVar20 | 0x4000;
    puVar20[0x17] = uVar9 & 0xfffff000 | local_f8 & 0xfff;
    puVar20[0x17] = uVar9 & 0xf000f000 | local_f8 & 0xfff | (uStack_f4 & 0xfff) << 0x10;
    puVar20[0x18] = uVar6 & 0xfffff000 | local_f0 & 0xfff;
    puVar20[0x18] = uVar6 & 0xf000f000 | local_f0 & 0xfff | (uStack_ec & 0xfff) << 0x10;
  }
  uVar9 = local_b8;
  uVar6 = local_c8;
  if (*(char *)(param_2 + 0x10e) != '\0') {
    local_50 = *(undefined4 *)(param_2 + 0xb0);
    local_4c = *(uint *)(param_2 + 0xb8);
    uVar10 = param_3[2];
    uVar11 = param_3[3];
    uVar12 = *param_3;
    uVar13 = param_3[1];
    local_54 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x5c));
    local_58 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x48));
    local_78 = *(uint *)(param_2 + 0x4c);
    local_74 = *(undefined4 *)(param_2 + 0x50);
    local_5c = (uint)*(byte *)(param_2 + 0x84);
    local_60 = (uint)*(byte *)(param_2 + 0x85);
    local_7c = puVar20[4] & 0xfff;
    local_88 = *(uint *)(param_2 + 0x60);
    local_8c = puVar20[0xd] & 0xfff;
    local_84 = *(undefined4 *)(param_2 + 100);
    local_80 = *(undefined4 *)(param_2 + 0xb4);
    local_90 = *(uint *)(param_2 + 0xbc);
    local_64 = (uint)*(byte *)(param_2 + 0x86);
    local_68 = (uint)*(byte *)(param_2 + 0x87);
    local_6c = hcge_blittingflags_to_string(*(undefined4 *)(param_2 + 0xc));
    local_70 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa0));
    uVar7 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa4));
    printf("%s() 0x%08lx --> 0x%08lx, {(%d, %d), (%d, %d)} - (%d X %d) @--> {(%d, %d) , (%d, %d)} - (%d x %d)\n\t\t\t\t\t\t\t %s --> %s, spitch(%d - %d) size (%d, %d) -> dpitch(%d - %d) size(%d, %d), state->color= (%02x | %02x_%02x_%02x), \n\t\t\t\t\t\t\t bflags = %s, src_blend = %s, dst_blend = %s&\n"
           ,"hcge_blit_no_matrix",local_4c,local_50,uVar12,uVar13,uVar12 + uVar10 + -1,
           uVar13 + uVar11 + -1,uVar10,uVar11,uVar9,uVar6,uVar10 + uVar9 + -1,uVar11 + uVar6 + -1,
           uVar10,uVar11,local_54,local_58,local_90,local_8c,local_88,local_84,local_80,local_7c,
           local_78,local_74,local_5c,local_60,local_64,local_68,local_6c,local_70,uVar7);
  }
  if (*(char *)(param_2 + 0x10c) == '\0') {
    uVar6 = puVar20[0xe];
    uVar11 = *puVar20;
    uVar10 = *param_3 & 0xfff;
    puVar20[0xe] = uVar6 & 0xfffff000 | uVar10;
    uVar9 = param_3[1];
    *puVar20 = uVar11 | 0x400;
    puVar20[0xe] = uVar6 & 0xf000f000 | uVar10 | (uVar9 & 0xfff) << 0x10;
    uVar6 = param_3[2];
    if ((int)puVar20[1] < 0) {
      uVar12 = puVar20[0xf];
      iVar5 = (uVar6 + 1) * 3;
      if (iVar5 < 0) {
        iVar5 = iVar5 + 3;
      }
      uVar13 = iVar5 >> 2 & 0xfff;
      puVar20[0xf] = uVar12 & 0xfffff000 | uVar13;
      uVar18 = param_3[3];
      puVar20[5] = puVar20[5] & 0xf000f000 | local_b8 & 0xfff | (local_c8 & 0xfff) << 0x10;
      uVar17 = uVar18 & 0xfff;
      puVar20[6] = puVar20[6] & 0xfffff000 | ((iVar5 >> 2) << 2) / 3 & 0xfffU;
      puVar20[0xf] = uVar12 & 0xf000f000 | uVar13 | uVar17 << 0x10;
    }
    else {
      uVar12 = puVar20[0xf];
      uVar13 = uVar6 & 0xfff;
      puVar20[0xf] = uVar12 & 0xfffff000 | uVar13;
      uVar18 = param_3[3];
      puVar20[6] = puVar20[6] & 0xfffff000 | uVar13;
      puVar20[5] = puVar20[5] & 0xf000f000 | local_b8 & 0xfff | (local_c8 & 0xfff) << 0x10;
      uVar17 = uVar18 & 0xfff;
      puVar20[0xf] = uVar12 & 0xf000f000 | uVar13 | uVar17 << 0x10;
    }
    uVar12 = uVar11 | 0x700;
    *puVar20 = uVar12;
    puVar20[6] = puVar20[6] & 0xf000ffff | uVar17 << 0x10;
    puVar20[10] = puVar20[10] & 0xf000f000 | local_b8 & 0xfff | (local_c8 & 0xfff) << 0x10;
    if (*(char *)(param_2 + 0x10f) != '\0') {
      uVar13 = *(uint *)(param_2 + 0x1c);
      uVar12 = uVar11 | 0xf00;
      *puVar20 = uVar12;
      if ((uVar13 & 1) == 0) {
        puVar20[0x13] = puVar20[0x13] & 0xf000f000 | uVar10 | (uVar9 & 0xfff) << 0x10;
        uVar9 = *(int *)(param_2 + 0x74) - uVar6;
        pcVar21 = puts;
        if ((int)uVar9 < 1) goto LAB_0001ab7c;
        uVar10 = puVar20[0x14];
        uVar9 = uVar9 & 0xfff;
        puVar20[0x14] = uVar10 & 0xfffff000 | uVar9;
        uVar11 = *(int *)(param_2 + 0x78) - uVar18;
        uVar9 = uVar10 & 0xf000f000 | uVar9 | (uVar11 & 0xfff) << 0x10;
      }
      else {
        uVar10 = *(uint *)(param_2 + 0x14);
        uVar13 = puVar20[0x13];
        uVar9 = uVar10 & 0xfff;
        puVar20[0x13] = uVar13 & 0xfffff000 | uVar9;
        uVar11 = *(uint *)(param_2 + 0x18);
        puVar20[0x13] = uVar13 & 0xf000f000 | uVar9 | (uVar11 & 0xfff) << 0x10;
        uVar10 = *(int *)(param_2 + 0x74) - uVar10;
        pcVar21 = printf;
        if ((int)uVar10 < 1) {
LAB_0001ab7c:
          (*pcVar21)("msk w is 0");
          puVar20[0x14] = puVar20[0x14] & 0xfffff000;
          return '\0';
        }
        uVar9 = puVar20[0x14];
        uVar10 = uVar10 & 0xfff;
        puVar20[0x14] = uVar9 & 0xfffff000 | uVar10;
        uVar11 = *(int *)(param_2 + 0x78) - uVar11;
        uVar9 = uVar9 & 0xf000f000 | uVar10 | (uVar11 & 0xfff) << 0x10;
      }
      if (uVar11 == 0) {
        (*pcVar21)("msk w is 0");
        puVar20[0x14] = puVar20[0x14] & 0xf000ffff;
        return '\0';
      }
      puVar20[0x14] = uVar9;
    }
    dVar22 = *(double *)(param_2 + 0x428);
    if (dVar22 != 0.0) {
      *puVar20 = uVar12 | 0x40000;
      uVar9 = puVar20[0xd] & 0xbfffffff | 0x40000000;
      puVar20[0xd] = uVar9;
      puVar20[1] = puVar20[1] & 0xffffcfff | 0x3000;
      if ((dVar22 == 90.0) || (dVar22 == 180.0)) {
        iVar5 = 0;
      }
      else {
        iVar5 = 0;
        if (dVar22 != 270.0) {
          iVar5 = 1;
          uVar11 = puVar20[0x23];
          uVar10 = (*(uint *)(param_2 + 0x430) & 0xfff) << 8;
          puVar20[0x23] = uVar11 & 0xfff000ff | uVar10;
          puVar20[0x23] = uVar11 & 0xff | uVar10 | *(int *)(param_2 + 0x434) << 0x14;
        }
      }
      puVar20[0x23] = puVar20[0x23] & 0xffffff0f | iVar5 << 6;
      hcge_matrix_init_rotate_ext
                (&local_f8,uVar9,SUB84(dVar22,0),(int)((ulonglong)dVar22 >> 0x20),uVar6,uVar18);
      if ((*(double *)(param_2 + 0x428) == 90.0) || (*(double *)(param_2 + 0x428) == 270.0)) {
        uVar9 = param_3[3];
        uVar6 = puVar20[6];
        puVar20[6] = uVar6 & 0xfffff000 | uVar9 & 0xfff;
        puVar20[6] = uVar6 & 0xf000f000 | uVar9 & 0xfff | (param_3[2] & 0xfff) << 0x10;
      }
      uVar6 = hcge_ge_coeff(CONCAT44(uStack_f4,local_f8));
      puVar20[0x24] = uVar6;
      uVar6 = hcge_ge_coeff(CONCAT44(uStack_ec,local_f0));
      puVar20[0x25] = uVar6;
      uVar6 = hcge_ge_coeff(local_e8);
      puVar20[0x26] = uVar6;
      uVar6 = hcge_ge_coeff(local_e0);
      puVar20[0x27] = uVar6;
      uVar6 = hcge_ge_coeff(local_d8);
      puVar20[0x28] = uVar6;
      uVar6 = hcge_ge_coeff(local_d0);
      puVar20[0x29] = uVar6;
    }
  }
  else {
    uVar9 = puVar20[6];
    uVar6 = puVar20[10];
    puVar20[5] = puVar20[5] & 0xf000f000 | local_b8 & 0xfff | (local_c8 & 0xfff) << 0x10;
    uVar10 = param_3[2];
    puVar20[6] = uVar9 & 0xfffff000 | uVar10 & 0xfff;
    uVar11 = param_3[3];
    *puVar20 = *puVar20 | 0x300;
    puVar20[6] = uVar9 & 0xf000f000 | uVar10 & 0xfff | (uVar11 & 0xfff) << 0x10;
    uVar9 = *param_3;
    puVar20[10] = uVar6 & 0xfffff000 | uVar9 & 0xfff;
    puVar20[10] = uVar6 & 0xf000f000 | uVar9 & 0xfff | (param_3[1] & 0xfff) << 0x10;
  }
  if (*(char *)(param_2 + 0x111) == '\0') {
    hcge_feed_nodes(param_2,matrix_disable_node_6155,&DAT_000332fc,"matrix_disable_node");
  }
  if ((*(int *)(param_2 + 0x5c) != 0x4011040b) || (*(int *)(param_2 + 0xc) << 0xf < 0))
  goto LAB_0001a054;
  iVar5 = *(int *)(param_2 + 0xec);
  puVar20 = *(uint **)(param_2 + 0x158);
  if (*(char *)(param_2 + 0x110) == '\0') {
    puVar19 = (uint *)(iVar5 + 0x20);
    *(undefined1 *)(param_2 + 0x110) = 1;
    if (*(int *)(param_2 + 0x68) == 0) goto LAB_0001a580;
    pbVar15 = *(byte **)(param_2 + 0x6c);
    uVar6 = 0;
    do {
      uVar6 = uVar6 + 1;
      *puVar19 = (uint)*pbVar15 << 0x18 | (uint)pbVar15[1] << 0x10 | (uint)pbVar15[3] |
                 (uint)pbVar15[2] << 8;
      pbVar15 = pbVar15 + 4;
      puVar19 = puVar19 + 1;
    } while (uVar6 < *(uint *)(param_2 + 0x68));
    uVar6 = puVar20[0x15];
  }
  else {
    puVar19 = (uint *)(iVar5 + 0x20);
    if (*(int *)(param_2 + 0x68) != 0) {
      pbVar15 = *(byte **)(param_2 + 0x6c);
      if (*(uint *)(iVar5 + 0x20) ==
          ((uint)*pbVar15 << 0x18 | (uint)pbVar15[1] << 0x10 | (uint)pbVar15[3] |
          (uint)pbVar15[2] << 8)) {
        puVar14 = (uint *)(iVar5 + 0x24);
        iVar5 = 0;
        do {
          pbVar16 = pbVar15 + 4;
          iVar5 = iVar5 + 1;
          if (iVar5 == *(int *)(param_2 + 0x68)) goto LAB_0001a580;
          pbVar1 = pbVar15 + 5;
          pbVar2 = pbVar15 + 6;
          pbVar3 = pbVar15 + 7;
          uVar6 = *puVar14;
          puVar14 = puVar14 + 1;
          pbVar15 = pbVar16;
        } while (((uint)*pbVar16 << 0x18 | (uint)*pbVar1 << 0x10 | (uint)*pbVar3 |
                 (uint)*pbVar2 << 8) == uVar6);
      }
      hcge_engine_sync(param_2);
      if (*(int *)(param_2 + 0x68) != 0) {
        pbVar15 = *(byte **)(param_2 + 0x6c);
        uVar6 = 0;
        do {
          uVar6 = uVar6 + 1;
          *puVar19 = (uint)*pbVar15 << 0x18 | (uint)pbVar15[1] << 0x10 | (uint)pbVar15[3] |
                     (uint)pbVar15[2] << 8;
          pbVar15 = pbVar15 + 4;
          puVar19 = puVar19 + 1;
        } while (uVar6 < *(uint *)(param_2 + 0x68));
        uVar6 = puVar20[0x15];
        goto LAB_0001a584;
      }
    }
LAB_0001a580:
    uVar6 = puVar20[0x15];
  }
LAB_0001a584:
  puVar20[1] = puVar20[1] & 0xfffbffff | 0x40000;
  *puVar20 = *puVar20 | 0x2000;
  puVar20[0x15] = uVar6 & 0x7c00ffff | 0x80550000;
  puVar20[0x16] = *(uint *)(param_2 + 0x100);
LAB_0001a054:
  local_f8 = param_2 + 0x15cU;
  hcge_construct_nodes(param_2,&local_f8);
  hcge_feed_nodes(param_2,param_2 + 0x15cU,local_f8,"hcge_blit_no_matrix");
  *(undefined1 *)(param_2 + 0x111) = 1;
  return '\x01';
}



/* ===== FUNCTION hcge_stretch_blit @ 0001ab98 ===== */

int hcge_stretch_blit(undefined8 param_1,int param_2,uint *param_3,int *param_4)

{
  uint *puVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined4 uVar7;
  uint uVar8;
  uint uVar9;
  uint *puVar10;
  uint *puVar11;
  uint uVar12;
  int iVar13;
  uint *puVar14;
  uint *puVar15;
  undefined8 uVar16;
  uint local_248 [64];
  undefined8 local_148;
  undefined8 local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined8 local_128;
  undefined8 local_120;
  undefined1 auStack_118 [48];
  double local_e8;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_cc;
  double local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  uint local_b8;
  uint local_b4;
  uint local_b0;
  uint local_ac;
  uint local_a8;
  uint local_a4;
  uint local_a0;
  uint local_9c;
  int local_98;
  int local_94;
  int local_90;
  uint local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  uint local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  uint local_6c;
  undefined4 local_68;
  undefined4 local_64;
  uint local_60;
  uint local_5c;
  uint local_58;
  undefined4 local_54;
  undefined4 local_50;
  uint local_4c;
  undefined4 local_48;
  undefined4 local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  uint local_30;
  undefined4 uVar17;
  
  uVar17 = (undefined4)((ulonglong)param_1 >> 0x20);
  if ((0x4000 < *(uint *)(param_2 + 0xbc)) || (0x4000 < *(uint *)(param_2 + 0xb4))) {
    return 0;
  }
  uVar12 = param_3[3];
  iVar13 = *(int *)(param_2 + 0x11c);
  puVar14 = *(uint **)(param_2 + 0x158);
  *param_4 = *param_4 + *(int *)(param_2 + 0x118);
  param_4[1] = param_4[1] + iVar13;
  fVar2 = (float)(int)uVar12;
  if ((int)uVar12 < 0) {
    fVar2 = fVar2 + 4.2949673e+09;
  }
  fVar3 = (float)(int)param_3[2];
  local_c8 = (double)(fVar2 / (float)param_4[3]);
  if ((int)param_3[2] < 0) {
    fVar3 = fVar3 + 4.2949673e+09;
  }
  local_e0 = 0;
  local_dc = 0;
  local_d8 = 0;
  local_d4 = 0;
  local_d0 = 0;
  local_cc = 0;
  local_c0 = 0;
  local_bc = 0;
  local_e8 = (double)(fVar3 / (float)param_4[2]);
  hcge_get_inverse_matrix(param_2 + 0x128,auStack_118);
  uVar16 = CONCAT44(uVar17,(float)-*param_4);
  hcge_matrix_translate_left_multiply(uVar16,(float)-param_4[1]);
  uVar17 = (undefined4)((ulonglong)uVar16 >> 0x20);
  hcge_matrix_multiply(&local_e8,auStack_118,&local_148);
  uVar16 = CONCAT44(uVar17,(float)(int)*param_3);
  hcge_matrix_translate_left_multiply(uVar16,(float)(int)param_3[1]);
  uVar17 = (undefined4)((ulonglong)uVar16 >> 0x20);
  iVar13 = hcge_get_bounding_rect(param_2,param_4,&local_a8);
  if ((iVar13 != 0) &&
     ((*(char *)(param_2 + 0x10d) == '\0' || (iVar4 = hcge_clip_rect(param_2,&local_a8), iVar4 != 0)
      ))) {
    iVar4 = hcge_clip_qw(&local_a8,&local_b8);
    uVar6 = local_a4;
    uVar12 = local_a8;
    if (iVar4 == 0) {
      uVar5 = *puVar14 & 0xffffbfff;
    }
    else {
      uVar9 = puVar14[0x17];
      uVar8 = puVar14[0x18];
      puVar14[0x17] = uVar9 & 0xfffff000 | local_b8 & 0xfff;
      uVar5 = *puVar14 | 0x4000;
      puVar14[0x17] = uVar9 & 0xf000f000 | local_b8 & 0xfff | (local_b4 & 0xfff) << 0x10;
      puVar14[0x18] = uVar8 & 0xfffff000 | local_b0 & 0xfff;
      puVar14[0x18] = uVar8 & 0xf000f000 | local_b0 & 0xfff | (local_ac & 0xfff) << 0x10;
    }
    *puVar14 = uVar5;
    puVar14[1] = puVar14[1] & 0xefffffff | (uint)(iVar4 != 0) << 0x1c;
    if (*(char *)(param_2 + 0x10e) != '\0') {
      local_90 = param_4[2];
      local_94 = param_4[1];
      local_98 = *param_4;
      local_48 = *(undefined4 *)(param_2 + 0xb0);
      local_30 = *param_3;
      local_34 = param_3[1];
      local_38 = param_3[2];
      local_8c = param_3[3];
      iVar4 = param_4[3];
      local_40 = local_9c;
      local_3c = local_a0;
      local_44 = *(undefined4 *)(param_2 + 0xb8);
      local_88 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x5c));
      local_84 = hcge_pixelformat_name(*(undefined4 *)(param_2 + 0x48));
      local_80 = *(undefined4 *)(param_2 + 0xbc);
      local_60 = (uint)*(byte *)(param_2 + 0x84);
      local_4c = (uint)*(byte *)(param_2 + 0x87);
      local_68 = *(undefined4 *)(param_2 + 0x4c);
      local_7c = puVar14[0xd] & 0xfff;
      local_64 = *(undefined4 *)(param_2 + 0x50);
      local_78 = *(undefined4 *)(param_2 + 0x60);
      local_74 = *(undefined4 *)(param_2 + 100);
      local_70 = *(undefined4 *)(param_2 + 0xb4);
      local_5c = (uint)*(byte *)(param_2 + 0x85);
      local_6c = puVar14[4] & 0xfff;
      local_58 = (uint)*(byte *)(param_2 + 0x86);
      local_50 = hcge_blittingflags_to_string(*(undefined4 *)(param_2 + 0xc));
      local_54 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa0));
      uVar7 = hcge_blend_to_string(*(undefined4 *)(param_2 + 0xa4));
      printf("%s() 0x%08lx --> 0x%08lx, {(%d, %d) , (%d, %d)} - (%d X %d) @--> {(%d, %d) , (%d, %d)} - (%d x %d), \n\t\t\t\t\t      $[brect] {(%4d, %4d) , (%4d, %4d)} - (%4d X %4d)$, %s -> %s, spitch(%d - %d) size (%d, %d) -> dpitch(%d - %d) size(%d, %d), color = (%02x | %02x_%02x_%02x), \n\t\t\t\t\t       bflags = %s, src_blend = %s, dst_blend = %s&\n"
             ,"hcge_stretch_blit",local_44,local_48,local_30,local_34,local_30 + local_38 + -1,
             local_34 + local_8c + -1,local_38,local_8c,local_98,local_94,local_98 + local_90 + -1,
             local_94 + iVar4 + -1,local_90,iVar4,uVar12,uVar6,uVar12 + local_3c + -1,
             uVar6 + local_40 + -1,local_3c,local_40,local_88,local_84,local_80,local_7c,local_78,
             local_74,local_70,local_6c,local_68,local_64,local_60,local_5c,local_58,local_4c,
             local_50,local_54,uVar7);
    }
    hcge_matrix_translate_right_multiply
              (CONCAT44(uVar17,(float)(int)((int)local_a0 / 2 + local_a8)),
               (float)(int)((int)local_9c / 2 + local_a4),local_a8,local_9c,&local_148);
    puVar14[0xd] = puVar14[0xd] & 0xbfffffff | 0x40000000;
    puVar14[1] = puVar14[1] & 0xffffcfff | 0x3000;
    puVar14[0x23] = puVar14[0x23] & 0xffffff3f | 0x80;
    *puVar14 = *puVar14 | 0x40000;
    uVar12 = hcge_ge_coeff(local_148);
    puVar14[0x24] = uVar12;
    uVar12 = hcge_ge_coeff(local_140);
    puVar14[0x25] = uVar12;
    uVar12 = hcge_ge_coeff(local_138);
    puVar14[0x26] = uVar12;
    uVar12 = hcge_ge_coeff(local_130);
    puVar14[0x27] = uVar12;
    uVar12 = hcge_ge_coeff(local_128);
    puVar14[0x28] = uVar12;
    uVar12 = hcge_ge_coeff(local_120);
    puVar14[0x29] = uVar12;
    if ((puVar14[0x23] & 0xc) == 4) {
      puVar15 = local_248;
      uVar12 = param_3[2];
      *puVar14 = *puVar14 | 0x80000;
      gen_fliter_coef(uVar12,local_a0,puVar15);
      puVar11 = puVar14 + 0x2a;
      uVar12 = 0;
      puVar10 = puVar15;
      do {
        iVar4 = uVar12 << 1;
        uVar6 = uVar12 & 1;
        uVar12 = uVar12 + 1;
        *puVar11 = (iVar4 >> 2) << 1 | uVar6 | 0x80000000;
        puVar1 = puVar10 + 1;
        uVar6 = *puVar10;
        puVar10 = puVar10 + 2;
        puVar11[1] = *puVar1 << 0x10 | uVar6;
        puVar11 = puVar11 + 2;
      } while (uVar12 != 0x20);
      gen_fliter_coef(param_3[3],local_9c,puVar15);
      puVar10 = puVar14 + 0x6a;
      uVar12 = 0;
      do {
        iVar4 = uVar12 << 1;
        uVar6 = uVar12 & 1;
        uVar12 = uVar12 + 1;
        *puVar10 = (iVar4 >> 2) << 1 | uVar6 | 0x80000020;
        puVar11 = puVar15 + 1;
        uVar6 = *puVar15;
        puVar15 = puVar15 + 2;
        puVar10[1] = *puVar11 << 0x10 | uVar6;
        puVar10 = puVar10 + 2;
      } while (uVar12 != 0x20);
      uVar12 = *param_3;
    }
    else {
      uVar12 = *param_3;
    }
    uVar9 = puVar14[0xe];
    uVar8 = puVar14[0xf];
    uVar5 = puVar14[5];
    uVar6 = puVar14[6];
    puVar14[0xe] = uVar9 & 0xfffff000 | uVar12 & 0xfff;
    puVar14[0xe] = uVar9 & 0xf000f000 | uVar12 & 0xfff | (param_3[1] & 0xfff) << 0x10;
    uVar12 = param_3[2];
    puVar14[0xf] = uVar8 & 0xfffff000 | uVar12 & 0xfff;
    puVar14[0xf] = uVar8 & 0xf000f000 | uVar12 & 0xfff | (param_3[3] & 0xfff) << 0x10;
    local_a8 = local_a8 & 0xfff;
    puVar14[5] = uVar5 & 0xfffff000 | local_a8;
    puVar14[5] = uVar5 & 0xf000f000 | local_a8 | (local_a4 & 0xfff) << 0x10;
    puVar14[6] = uVar6 & 0xfffff000 | local_a0 & 0xfff;
    puVar14[10] = puVar14[10] & 0xf000f000 | local_a8 | (local_a4 & 0xfff) << 0x10;
    *puVar14 = *puVar14 | 0x700;
    puVar14[6] = uVar6 & 0xf000f000 | local_a0 & 0xfff | (local_9c & 0xfff) << 0x10;
    if (*(char *)(param_2 + 0x111) != '\0') {
      matrix_fix_node_6149._8_4_ = puVar14[2] & 0xfffffff;
      hcge_feed_nodes(param_2,matrix_fix_node_6149,matrix_disable_node_6155,"HW fix node");
    }
    local_248[0] = param_2 + 0x15cU;
    hcge_construct_nodes(param_2,local_248);
    hcge_feed_nodes(param_2,param_2 + 0x15cU,local_248[0],"hcge_stretch_blit");
    *(undefined1 *)(param_2 + 0x111) = 0;
    return iVar13;
  }
  return 1;
}



/* ===== FUNCTION hcge_fill_rect_ext @ 0001b44c ===== */

void hcge_fill_rect_ext(undefined4 *param_1,uint *param_2,undefined4 *param_3,undefined4 param_4,
                       uint *param_5)

{
  uint uVar1;
  uint *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  void *__ptr;
  int iVar5;
  uint uVar6;
  uint uVar7;
  size_t __size;
  
  param_1[2] = 1;
  param_1[1] = 0;
  *param_1 = 0;
  param_1[4] = 0x30001;
  if (param_3 != (undefined4 *)0x0) {
    uVar4 = param_3[1];
    uVar3 = param_3[2];
    iVar5 = param_3[3];
    param_1[0x12] = *param_3;
    param_1[0x13] = uVar4;
    param_1[0x14] = uVar3;
    if (iVar5 != 0) {
      __ptr = (void *)param_1[0x16];
      if (iVar5 == param_1[0x15]) {
        __size = iVar5 << 2;
      }
      else {
        param_1[0x15] = iVar5;
        if (__ptr != (void *)0x0) {
          free(__ptr);
        }
        __size = param_3[3] << 2;
        __ptr = malloc(__size);
        param_1[0x16] = __ptr;
      }
      memcpy(__ptr,(void *)param_3[4],__size);
      uVar3 = param_1[2];
      goto LAB_0001b52c;
    }
  }
  uVar3 = 1;
LAB_0001b52c:
  param_1[3] = 0;
  param_1[0x28] = 5;
  param_1[0x29] = 1;
  uVar6 = *param_5;
  uVar1 = (int)param_1 + 0x87U & 3;
  puVar2 = (uint *)(((int)param_1 + 0x87U) - uVar1);
  *puVar2 = *puVar2 & -1 << (uVar1 + 1) * 8 | uVar6 >> (3 - uVar1) * 8;
  uVar1 = (uint)(param_1 + 0x21) & 3;
  puVar2 = (uint *)((int)(param_1 + 0x21) - uVar1);
  *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar1) * 8 | uVar6 << uVar1 * 8;
  uVar7 = *param_2;
  uVar6 = param_2[1];
  uVar1 = (int)param_1 + 0xb3U & 3;
  puVar2 = (uint *)(((int)param_1 + 0xb3U) - uVar1);
  *puVar2 = *puVar2 & -1 << (uVar1 + 1) * 8 | uVar7 >> (3 - uVar1) * 8;
  uVar1 = (uint)(param_1 + 0x2c) & 3;
  puVar2 = (uint *)((int)(param_1 + 0x2c) - uVar1);
  *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar1) * 8 | uVar7 << uVar1 * 8;
  param_1[0x2d] = uVar6;
  param_1[0x33] = 0;
  param_1[0x32] = 4;
  param_1[0x34] = 0;
  param_1[0x35] = 0x4ff;
  param_1[0x36] = 0x2cf;
  hcge_set_state(param_1,param_1,uVar3);
                    /* WARNING: Could not recover jumptable at 0x0001b5d4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  hcge_fill_rect(param_1,param_4);
  return;
}



/* ===== FUNCTION hcge_ge_coeff @ 0001b5f0 ===== */

uint hcge_ge_coeff(double param_1)

{
  uint uVar1;
  double dVar2;
  
  if (param_1 < 0.0) {
    dVar2 = -param_1 * 65536.0;
    if (dVar2 < 2147483648.0) {
      uVar1 = (uint)dVar2;
    }
    else {
      uVar1 = (int)(dVar2 - 2147483648.0) | 0x80000000;
    }
    if ((int)uVar1 < 0) {
      return 0xffffffff;
    }
  }
  else {
    dVar2 = param_1 * 65536.0;
    if (dVar2 < 2147483648.0) {
      uVar1 = (uint)dVar2;
    }
    else {
      uVar1 = (int)(dVar2 - 2147483648.0) | 0x80000000;
    }
    if ((int)uVar1 < 0) {
      uVar1 = 0x7fffffff;
    }
    if (0.0 <= param_1) {
      return uVar1;
    }
  }
  return uVar1 | 0x80000000;
}



/* ===== FUNCTION hcge_clip_rect @ 0001b6f0 ===== */

undefined4 hcge_clip_rect(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar2 = *param_2;
  iVar3 = *(int *)(param_1 + 0xcc);
  if ((iVar3 < iVar2 + param_2[2]) && (iVar5 = *(int *)(param_1 + 0xd4), iVar2 <= iVar5)) {
    iVar6 = param_2[1];
    iVar7 = *(int *)(param_1 + 0xd0);
    if (iVar7 < iVar6 + param_2[3]) {
      iVar8 = *(int *)(param_1 + 0xd8);
      iVar4 = iVar6 + param_2[3] + -1;
      if (iVar6 <= iVar8) {
        iVar1 = iVar2 + param_2[2] + -1;
        if (iVar1 <= iVar5) {
          iVar5 = iVar1;
        }
        if (iVar6 < 0) {
          iVar6 = 0;
        }
        if (iVar8 < iVar4) {
          iVar4 = iVar8;
        }
        if (iVar2 < 0) {
          iVar2 = 0;
        }
        if (*(int *)(param_1 + 0x4c) <= iVar5) {
          iVar5 = *(int *)(param_1 + 0x4c) + -1;
        }
        if (*(int *)(param_1 + 0x50) <= iVar4) {
          iVar4 = *(int *)(param_1 + 0x50) + -1;
        }
        if (iVar3 <= iVar2) {
          iVar3 = iVar2;
        }
        if (iVar7 <= iVar6) {
          iVar7 = iVar6;
        }
        *param_2 = iVar3;
        param_2[1] = iVar7;
        param_2[2] = (iVar5 - iVar3) + 1;
        param_2[3] = (iVar4 - iVar7) + 1;
        return 1;
      }
    }
  }
  return 0;
}



/* ===== FUNCTION hcge_clip_blit @ 0001b7d4 ===== */

void hcge_clip_blit(undefined4 param_1,int *param_2,int *param_3,int *param_4)

{
  int iVar1;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  local_18 = param_2[2];
  local_20 = *param_3;
  local_14 = param_2[3];
  local_1c = *param_4;
  iVar1 = hcge_clip_rect(param_1,&local_20);
  if (iVar1 != 0) {
    *param_2 = *param_2 + (local_20 - *param_3);
    iVar1 = *param_4;
    param_2[2] = local_18;
    param_2[3] = local_14;
    param_2[1] = param_2[1] + (local_1c - iVar1);
    *param_3 = local_20;
    *param_4 = local_1c;
  }
  return;
}



/* ===== FUNCTION hcge_clip_stretch_blit @ 0001b894 ===== */

void hcge_clip_stretch_blit(undefined4 param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = *param_3;
  iVar4 = param_3[1];
  iVar3 = param_3[2];
  iVar2 = param_3[3];
  iVar1 = hcge_clip_rect(param_1,param_3);
  if (iVar1 != 0) {
    if (iVar5 != *param_3) {
      *param_2 = *param_2 + (int)(((float)param_2[2] / (float)iVar3) * (float)(*param_3 - iVar5));
    }
    if (param_3[1] != iVar4) {
      param_2[1] = param_2[1] +
                   (int)(((float)param_2[3] / (float)iVar2) * (float)(param_3[1] - iVar4));
    }
    if (param_3[2] != iVar3) {
      param_2[2] = (int)(((float)param_3[2] / (float)iVar3) * (float)param_2[2]);
    }
    if (param_3[3] != iVar2) {
      param_2[3] = (int)(((float)param_3[3] / (float)iVar2) * (float)param_2[3]);
    }
  }
  return;
}



/* ===== FUNCTION hcge_get_inverse_matrix @ 0001b9f4 ===== */

void hcge_get_inverse_matrix(double *param_1,double *param_2)

{
  double dVar1;
  
  dVar1 = param_1[4] * *param_1 - param_1[1] * param_1[3];
  *param_2 = param_1[4] / dVar1;
  param_2[1] = -param_1[1] / dVar1;
  param_2[2] = (param_1[1] * param_1[5] - param_1[4] * param_1[2]) / dVar1;
  param_2[3] = -param_1[3] / dVar1;
  param_2[4] = *param_1 / dVar1;
  param_2[5] = (param_1[3] * param_1[2] - *param_1 * param_1[5]) / dVar1;
  return;
}



/* ===== FUNCTION hcge_matrix_translate_right_multiply @ 0001baa0 ===== */

void hcge_matrix_translate_right_multiply
               (float param_1,float param_2,undefined4 param_3,undefined4 param_4,double *param_5)

{
  param_5[2] = param_5[2] + (double)param_1 * *param_5 + (double)param_2 * param_5[1];
  param_5[5] = param_5[5] + (double)param_1 * param_5[3] + (double)param_2 * param_5[4];
  return;
}



/* ===== FUNCTION hcge_matrix_translate_left_multiply @ 0001bae4 ===== */

void hcge_matrix_translate_left_multiply
               (float param_1,float param_2,undefined4 param_3,undefined4 param_4,int param_5)

{
  *(double *)(param_5 + 0x10) = *(double *)(param_5 + 0x10) + (double)param_1;
  *(double *)(param_5 + 0x28) = *(double *)(param_5 + 0x28) + (double)param_2;
  return;
}



/* ===== FUNCTION hcge_matrix_multiply @ 0001bb08 ===== */

void hcge_matrix_multiply(double *param_1,double *param_2,double *param_3)

{
  *param_3 = *param_1 * *param_2 + param_1[1] * param_2[3];
  param_3[1] = *param_1 * param_2[1] + param_1[1] * param_2[4];
  param_3[2] = *param_1 * param_2[2] + param_1[1] * param_2[5];
  param_3[3] = param_1[3] * *param_2 + param_1[4] * param_2[3];
  param_3[4] = param_1[3] * param_2[1] + param_1[4] * param_2[4];
  param_3[5] = param_1[3] * param_2[2] + param_1[4] * param_2[5];
  return;
}



/* ===== FUNCTION hcge_get_bounding_rect @ 0001bbb4 ===== */

undefined4 hcge_get_bounding_rect(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  double dVar4;
  float fVar5;
  int iVar6;
  double dVar7;
  double dVar8;
  float fVar9;
  double dVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  double dVar14;
  float fVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  double dVar19;
  float fVar20;
  double dVar21;
  float fVar22;
  float fVar23;
  double dVar24;
  float fVar25;
  
  dVar18 = *(double *)(param_1 + 0x138);
  dVar16 = *(double *)(param_1 + 0x150);
  dVar19 = (double)*param_2;
  dVar24 = *(double *)(param_1 + 0x128) * dVar19;
  dVar10 = (double)param_2[1];
  dVar8 = (double)(param_2[1] + param_2[3] + -1);
  dVar19 = *(double *)(param_1 + 0x140) * dVar19;
  dVar4 = (double)(*param_2 + param_2[2] + -1);
  dVar21 = *(double *)(param_1 + 0x130) * dVar10;
  dVar10 = *(double *)(param_1 + 0x148) * dVar10;
  dVar7 = *(double *)(param_1 + 0x130) * dVar8;
  dVar14 = *(double *)(param_1 + 0x128) * dVar4;
  dVar8 = *(double *)(param_1 + 0x148) * dVar8;
  dVar4 = *(double *)(param_1 + 0x140) * dVar4;
  fVar11 = (float)(dVar24 + dVar21 + dVar18);
  fVar13 = (float)(dVar14 + dVar7 + dVar18);
  fVar22 = (float)(dVar19 + dVar10 + dVar16);
  fVar25 = (float)(dVar4 + dVar8 + dVar16);
  fVar20 = (float)(dVar21 + dVar14 + dVar18);
  fVar17 = (float)(dVar24 + dVar7 + dVar18);
  fVar23 = fVar13;
  if (fVar20 < fVar11) {
    fVar23 = fVar11;
  }
  fVar9 = (float)(dVar10 + dVar4 + dVar16);
  fVar15 = (float)(dVar19 + dVar8 + dVar16);
  fVar12 = fVar20;
  fVar5 = fVar22;
  if (fVar20 >= fVar11) {
    fVar12 = fVar11;
    fVar5 = fVar25;
  }
  fVar11 = fVar9;
  if (fVar22 <= fVar9) {
    fVar11 = fVar22;
  }
  if (fVar17 < fVar12) {
    fVar12 = fVar17;
    fVar5 = fVar9;
    fVar23 = fVar20;
  }
  fVar20 = fVar15;
  if (fVar11 <= fVar15) {
    fVar20 = fVar11;
  }
  if (fVar15 <= fVar5) {
    fVar17 = fVar23;
  }
  if (fVar13 < fVar12) {
    fVar12 = fVar13;
    fVar5 = fVar15;
  }
  fVar23 = fVar25;
  if (fVar20 <= fVar25) {
    fVar23 = fVar20;
  }
  if (fVar25 <= fVar5) {
    fVar13 = fVar17;
  }
  fVar17 = (float)(*(int *)(param_1 + 0x4c) + -1);
  if ((((fVar12 <= fVar17) && (0.0 <= fVar13)) &&
      (fVar11 = (float)(*(int *)(param_1 + 0x50) + -1), fVar23 <= fVar11)) && (0.0 <= fVar5)) {
    iVar1 = 0;
    if (0.0 < fVar12) {
      iVar1 = (int)fVar12;
    }
    if (fVar17 <= fVar13) {
      fVar13 = fVar17;
    }
    iVar2 = 0;
    if (0.0 < fVar23) {
      iVar2 = (int)fVar23;
    }
    if (fVar11 <= fVar5) {
      fVar5 = fVar11;
    }
    *param_3 = iVar1;
    iVar6 = (int)fVar13;
    param_3[1] = iVar2;
    if ((float)iVar6 < fVar13) {
      iVar6 = iVar6 + 1;
    }
    iVar3 = (int)fVar5;
    param_3[2] = (iVar6 - iVar1) + 1;
    if ((float)iVar3 < fVar5) {
      iVar3 = iVar3 + 1;
    }
    param_3[3] = (iVar3 - iVar2) + 1;
    return 1;
  }
  return 0;
}



/* ===== FUNCTION hcge_matrix_init_rotate @ 0001be60 ===== */

void hcge_matrix_init_rotate
               (double *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 local_18;
  uint local_14;
  double local_10;
  
  sincos(((double)CONCAT44(param_4,param_3) * 3.141592502593994) / 180.0,(double *)&local_18,
         &local_10);
  *(undefined4 *)(param_1 + 2) = 0;
  *(undefined4 *)((int)param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 1) = local_18;
  *(uint *)((int)param_1 + 0xc) = local_14 ^ 0x80000000;
  *(undefined4 *)(param_1 + 3) = local_18;
  *(uint *)((int)param_1 + 0x1c) = local_14;
  *(undefined4 *)(param_1 + 5) = 0;
  *(undefined4 *)((int)param_1 + 0x2c) = 0;
  *param_1 = local_10;
  param_1[4] = local_10;
  return;
}



/* ===== FUNCTION hcge_matrix_init_rotate_ext @ 0001befc ===== */

void hcge_matrix_init_rotate_ext
               (double *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               uint param_5,uint param_6)

{
  double dVar1;
  double dVar2;
  undefined4 local_28;
  uint local_24;
  double local_20;
  
  dVar2 = 180.0;
  dVar1 = (double)CONCAT44(param_4,param_3);
  sincos((dVar1 * 3.141592502593994) / 180.0,(double *)&local_28,&local_20);
  *(undefined4 *)(param_1 + 2) = 0;
  *(undefined4 *)((int)param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 5) = 0;
  *(undefined4 *)((int)param_1 + 0x2c) = 0;
  *(uint *)((int)param_1 + 0xc) = local_24 ^ 0x80000000;
  *(undefined4 *)(param_1 + 1) = local_28;
  *(undefined4 *)(param_1 + 3) = local_28;
  *(uint *)((int)param_1 + 0x1c) = local_24;
  *param_1 = local_20;
  param_1[4] = local_20;
  if (dVar1 == 90.0) {
    if ((param_5 & 1) == 0) {
      param_1[2] = -1.0;
      return;
    }
  }
  else if (dVar1 == 270.0) {
    if ((param_6 & 1) == 0) {
      param_1[5] = -1.0;
      return;
    }
  }
  else if (dVar1 == dVar2) {
    if ((param_5 & 1) == 0) {
      dVar1 = -1.0;
    }
    else {
      dVar1 = 0.0;
    }
    param_1[2] = dVar1;
    if ((param_6 & 1) == 0) {
      param_1[5] = -1.0;
      return;
    }
  }
  return;
}



/* ===== FUNCTION hcge_clip_qw @ 0001c078 ===== */

undefined4 hcge_clip_qw(int *param_1,int *param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar4 = param_1[2];
  iVar2 = *param_1 + iVar4;
  uVar1 = iVar2 + 7U;
  if ((int)(iVar2 + 7U) < 0) {
    uVar1 = iVar2 + 0xe;
  }
  iVar3 = (uVar1 & 0xfffffff8) - 1;
  if (iVar2 + -1 != iVar3) {
    iVar2 = param_1[3];
    *param_2 = *param_1;
    iVar6 = *param_1;
    iVar5 = param_1[1];
    param_2[2] = iVar4;
    param_2[1] = iVar5;
    param_1[2] = (iVar3 - iVar6) + 1;
    param_2[3] = iVar2;
    return 1;
  }
  return 0;
}



/* ===== FUNCTION grp_func_filler @ 0001c0e0 ===== */

void grp_func_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 4);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_dst_ctx_filler @ 0001c0f8 ===== */

void grp_dst_ctx_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  hcge_dst_addr = *(undefined4 *)(param_1 + 8);
  *puVar1 = hcge_dst_addr;
  puVar1[1] = *(undefined4 *)(param_1 + 0x10);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_src_ctx_filler @ 0001c12c ===== */

void grp_src_ctx_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x1c);
  puVar1[1] = *(undefined4 *)(param_1 + 0x24);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_ptn_ctx_filler @ 0001c14c ===== */

void grp_ptn_ctx_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x2c);
  puVar1[1] = *(undefined4 *)(param_1 + 0x34);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_msk_ctx_filler @ 0001c16c ===== */

void grp_msk_ctx_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x40);
  puVar1[1] = *(undefined4 *)(param_1 + 0x48);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_color_filler @ 0001c18c ===== */

void grp_color_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x70);
  puVar1[1] = *(undefined4 *)(param_1 + 0x74);
  puVar1[2] = *(undefined4 *)(param_1 + 0x78);
  *param_2 = (int)(puVar1 + 3);
  return;
}



/* ===== FUNCTION grp_paint_type_filler @ 0001c1b4 ===== */

void grp_paint_type_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x84);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_color_key_filler @ 0001c1cc ===== */

void grp_color_key_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x68);
  puVar1[1] = *(undefined4 *)(param_1 + 0x6c);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_dst_pos_filler @ 0001c1ec ===== */

void grp_dst_pos_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x14);
  hcge_dst_x = *(uint *)(param_1 + 0x14) & 0xfff;
  puVar1[1] = *(undefined4 *)(param_1 + 0x18);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_src_pos_filler @ 0001c228 ===== */

void grp_src_pos_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x28);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_ptn_pos_filler @ 0001c240 ===== */

void grp_ptn_pos_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x38);
  puVar1[1] = *(undefined4 *)(param_1 + 0x3c);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_msk_pos_filler @ 0001c260 ===== */

void grp_msk_pos_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x4c);
  puVar1[1] = *(undefined4 *)(param_1 + 0x50);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_clut_filler @ 0001c280 ===== */

void grp_clut_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x54);
  puVar1[1] = *(undefined4 *)(param_1 + 0x58);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_clip_filler @ 0001c2a0 ===== */

void grp_clip_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x5c);
  puVar1[1] = *(undefined4 *)(param_1 + 0x60);
  *param_2 = (int)(puVar1 + 2);
  return;
}



/* ===== FUNCTION grp_rop_filler @ 0001c2c0 ===== */

void grp_rop_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 100);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_dfb_gcolor_filler @ 0001c2d8 ===== */

void grp_dfb_gcolor_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x88);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_matrix_filler @ 0001c2f0 ===== */

void grp_matrix_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x8c);
  puVar1[1] = *(undefined4 *)(param_1 + 0x90);
  puVar1[2] = *(undefined4 *)(param_1 + 0x94);
  puVar1[3] = *(undefined4 *)(param_1 + 0x98);
  puVar1[4] = *(undefined4 *)(param_1 + 0x9c);
  puVar1[5] = *(undefined4 *)(param_1 + 0xa0);
  puVar1[6] = *(undefined4 *)(param_1 + 0xa4);
  *param_2 = (int)(puVar1 + 7);
  return;
}



/* ===== FUNCTION grp_flt_filler @ 0001c338 ===== */

void grp_flt_filler(int param_1,int *param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  
  puVar1 = (undefined4 *)*param_2;
  puVar5 = (undefined4 *)(param_1 + 0xa8);
  puVar2 = puVar1;
  do {
    uVar4 = *puVar5;
    puVar3 = puVar2 + 1;
    puVar5 = puVar5 + 1;
    *puVar2 = uVar4;
    puVar2 = puVar3;
  } while (puVar3 != puVar1 + 0x80);
  *param_2 = (int)puVar3;
  return;
}



/* ===== FUNCTION grp_dither_par0 @ 0001c360 ===== */

void grp_dither_par0(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x7c);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION grp_dither_par1 @ 0001c378 ===== */

void grp_dither_par1(int param_1,int *param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_2;
  *puVar1 = *(undefined4 *)(param_1 + 0x80);
  *param_2 = (int)(puVar1 + 1);
  return;
}



/* ===== FUNCTION hcge_cmdq_vaddr @ 0001c390 ===== */

int hcge_cmdq_vaddr(int param_1,uint param_2)

{
  if (*(uint *)(param_1 + 0xf4) <= param_2) {
    return (param_2 + *(int *)(param_1 + 0xf8)) - *(uint *)(param_1 + 0xf4);
  }
  printf("%s() : invalidate phyaddr = 0x%8x, while buf_phyaddr = 0x%08x\n","hcge_cmdq_vaddr",param_2
        );
  return 0;
}



/* ===== FUNCTION hcge_cmdq_paddr @ 0001c3f0 ===== */

int hcge_cmdq_paddr(int param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = *(uint *)(param_1 + 0xf8);
  iVar1 = *(int *)(param_1 + 0xf4);
  if (param_2 < uVar2) {
    printf("%s() : invlidate vaddr = 0x%08x, while buf_vaddr = 0x%08x\n","hcge_cmdq_paddr",param_2,
           uVar2);
  }
  return (param_2 + iVar1) - uVar2;
}



/* ===== FUNCTION hcge_construct_nodes @ 0001c45c ===== */

void hcge_construct_nodes(int param_1,int *param_2)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined **ppuVar6;
  uint *puVar7;
  code *pcVar8;
  
  puVar1 = (uint *)*param_2;
  puVar7 = *(uint **)(param_1 + 0x158);
  uVar2 = puVar7[2];
  uVar5 = puVar7[8];
  uVar4 = puVar7[0xc];
  uVar3 = puVar7[0x11];
  if ((uVar2 & 0xf0000000) != 0) {
    puVar1[1] = puVar7[3];
    *puVar1 = ((uVar2 >> 0x1c) + 0x2f) * 0x400 | 0x81000001;
    puVar1 = puVar1 + 2;
  }
  uVar2 = puVar7[7];
  if ((uVar2 & 0xf0000000) != 0) {
    puVar1[1] = uVar5;
    *puVar1 = ((uVar2 >> 0x1c) + 0x2f) * 0x400 | 0x81000001;
    puVar1 = puVar1 + 2;
  }
  uVar2 = puVar7[0xb];
  if ((uVar2 & 0xf0000000) != 0) {
    puVar1[1] = uVar4;
    *puVar1 = ((uVar2 >> 0x1c) + 0x2f) * 0x400 | 0x81000001;
    puVar1 = puVar1 + 2;
  }
  uVar2 = puVar7[0x10];
  if ((uVar2 & 0xf0000000) != 0) {
    puVar1[1] = uVar3;
    *puVar1 = ((uVar2 >> 0x1c) + 0x2f) * 0x400 | 0x81000001;
    puVar1 = puVar1 + 2;
  }
  ppuVar6 = &grp_filler;
  uVar2 = 0;
  *puVar1 = *puVar7 | 0x2000000;
  *param_2 = (int)(puVar1 + 1);
  do {
    while (uVar3 = uVar2 & 0x1f, uVar2 = uVar2 + 1, (1 << uVar3 & *puVar7) != 0) {
      pcVar8 = (code *)*ppuVar6;
      ppuVar6 = ppuVar6 + 1;
      (*pcVar8)(puVar7,param_2);
      if (uVar2 == 0x14) {
        return;
      }
    }
    ppuVar6 = ppuVar6 + 1;
  } while (uVar2 != 0x14);
  return;
}



/* ===== FUNCTION hcge_reset @ 0001c600 ===== */

void hcge_reset(int param_1)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  hcge_hw_reset();
  uVar2 = *(uint *)(param_1 + 0xf4);
  iVar3 = *(int *)(param_1 + 0xfc);
  iVar4 = *(int *)(param_1 + 0xe4);
  puVar1 = *(uint **)(param_1 + 0xec);
  *puVar1 = uVar2;
  puVar1[1] = iVar3 + uVar2;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 1;
  puVar1[6] = uVar2 & 0xfffffffe;
  *(uint *)(iVar4 + 0x10) = uVar2;
  *(uint *)(iVar4 + 0x14) = uVar2;
  return;
}



/* ===== FUNCTION hcge_start @ 0001c678 ===== */

void hcge_start(int param_1)

{
  uint *puVar1;
  uint uVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  
  puVar3 = *(uint **)(param_1 + 0xec);
  puVar1 = *(uint **)(param_1 + 0xe4);
  if ((puVar3[6] & 1) == 0) {
    if ((puVar3[6] & 0xfffffffe) <= puVar1[4]) {
      hcge_hw_reset();
      uVar2 = *(uint *)(param_1 + 0xf4);
      iVar4 = *(int *)(param_1 + 0xfc);
      iVar5 = *(int *)(param_1 + 0xe4);
      puVar1 = *(uint **)(param_1 + 0xec);
      *puVar1 = uVar2;
      puVar1[1] = iVar4 + uVar2;
      puVar1[2] = 0;
      puVar1[3] = 0;
      puVar1[4] = 1;
      puVar1[6] = uVar2 & 0xfffffffe;
      *(uint *)(iVar5 + 0x10) = uVar2;
      *(uint *)(iVar5 + 0x14) = uVar2;
      return;
    }
    puVar1[5] = (puVar3[6] & 0xfffffffe) - 4;
    *puVar1 = *puVar1 & 0x7fffffff | 0x80000000;
    *puVar1 = *puVar1 & 0xfffdffff | 0x20000;
    puVar3[2] = 0;
    puVar1[1] = puVar1[1] & 0xfffffffc | 2;
    return;
  }
  if (puVar3[5] <= puVar1[4]) {
    puVar1[4] = *puVar3;
    puVar1[5] = (puVar3[6] & 0xfffffffe) - 4;
    puVar3[6] = puVar3[6] & 0xfffffffe;
    *puVar1 = *puVar1 & 0x7fffffff | 0x80000000;
    *puVar1 = *puVar1 & 0xfffdffff | 0x20000;
    puVar3[2] = 0;
    puVar1[1] = puVar1[1] & 0xfffffffc | 2;
    return;
  }
  puVar1[5] = puVar3[5] - 4;
  *puVar1 = *puVar1 & 0x7fffffff | 0x80000000;
  *puVar1 = *puVar1 & 0xfffdffff | 0x20000;
  puVar3[2] = 0;
  puVar1[1] = puVar1[1] & 0xfffffffc | 2;
  return;
}



/* ===== FUNCTION hcge_feed_nodes @ 0001c7b4 ===== */

void hcge_feed_nodes(int param_1,void *param_2,int param_3)

{
  uint uVar1;
  void *pvVar2;
  uint *puVar3;
  int *piVar4;
  uint __n;
  int iVar5;
  
  __n = param_3 - (int)param_2;
  if ((__n & 3) != 0) {
    (*(code *)(undefined *)0x0)("%s() node length is not in word size\n","hcge_feed_nodes");
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
    (*(code *)0x2b00)();
    return;
  }
  piVar4 = *(int **)(param_1 + 0xec);
  if ((piVar4[4] != 0) || (piVar4[2] != 0)) {
    piVar4[6] = *piVar4;
    uVar1 = piVar4[6] & 0xfffffffe;
    pvVar2 = (void *)hcge_cmdq_vaddr(param_1,uVar1);
    memcpy(pvVar2,param_2,__n);
    puVar3 = *(uint **)(param_1 + 0xe4);
    puVar3[4] = uVar1;
    puVar3[5] = (uVar1 - 4) + __n;
    piVar4[4] = 0;
    piVar4[2] = 0;
    piVar4[6] = __n + piVar4[6];
    *puVar3 = *puVar3 & 0x7fffffff | 0x80000000;
    *puVar3 = *puVar3 & 0xfffdffff | 0x20000;
    puVar3[1] = puVar3[1] & 0xfffffffc | 2;
    return;
  }
  iVar5 = *(int *)(param_1 + 0xe4);
  do {
    ioctl(*(int *)(param_1 + 0x108),0x20002301,0);
    if ((piVar4[6] & 1U) == 0) {
      if (__n <= (uint)(piVar4[1] - piVar4[6])) {
        pvVar2 = (void *)hcge_cmdq_vaddr(param_1,piVar4[6],0);
        memcpy(pvVar2,param_2,__n);
        if ((piVar4[6] & 1U) != 0) {
          puts("Tail wrapped!!!");
        }
        goto LAB_0001c908;
      }
      if (*piVar4 + 8 + __n < *(uint *)(iVar5 + 0x10)) {
        pvVar2 = (void *)hcge_cmdq_vaddr(param_1,*piVar4,0);
        memcpy(pvVar2,param_2,__n);
        piVar4[5] = piVar4[6] & 0xfffffffe;
        piVar4[6] = __n + *piVar4 | 1;
        goto LAB_0001c918;
      }
    }
    else if ((int)(__n + 7) < (int)(*(int *)(iVar5 + 0x10) - (piVar4[6] & 0xfffffffeU))) {
      pvVar2 = (void *)hcge_cmdq_vaddr(param_1,piVar4[6] & 0xfffffffe,0);
      memcpy(pvVar2,param_2,__n);
      if ((piVar4[6] & 1U) == 0) {
        puts("Wrapped with no flag!!!");
      }
LAB_0001c908:
      piVar4[6] = __n + piVar4[6];
LAB_0001c918:
      ioctl(*(int *)(param_1 + 0x108),0x20002302,0);
      if (piVar4[2] == 0) {
        return;
      }
                    /* WARNING: Could not recover jumptable at 0x0001c964. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      hcge_start(param_1);
      return;
    }
    ioctl(*(int *)(param_1 + 0x108),0x20002302);
    usleep(100);
  } while( true );
}



/* ===== FUNCTION to_argb8888 @ 0001cb20 ===== */

uint to_argb8888(undefined4 param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  switch(param_1) {
  default:
    return param_2;
  case 2:
  case 3:
    uVar1 = (param_2 << 0x14) >> 0x1c;
    uVar2 = (param_2 << 0x18) >> 0x1c;
    return (param_2 & 0xf) << 4 | param_2 & 0xf | uVar1 << 0x14 | uVar1 << 0x10 | uVar2 << 0xc |
           uVar2 << 8;
  case 4:
  case 5:
    return (param_2 & 0x7000) << 4 | (param_2 & 0x380) << 1 | (param_2 & 0x1f) >> 2 |
           (param_2 & 0x3fc00) << 9 | (param_2 & 0x1fe0) << 6 | (param_2 & 0x1f) << 3;
  case 6:
    return (param_2 & 0x1f) << 3 | (param_2 & 0x1f) >> 2 | ((param_2 << 0x10) >> 0x1b) << 0x13 |
           ((param_2 << 0x15) >> 0x1a) << 10 | ((param_2 << 0x10) >> 0x1d) << 0x10 |
           ((param_2 << 0x15) >> 0x1e) << 8;
  }
}



/* ===== FUNCTION hcge_check_blit_state @ 0001cc2c ===== */

void hcge_check_blit_state(undefined4 param_1,uint *param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  if ((param_3 & 0x30000) == 0) {
    return;
  }
  uVar3 = param_2[3];
  if (uVar3 == 0) {
    uVar1 = param_2[0x17];
  }
  else {
    if ((uVar3 & 0x31176ff) == 0) {
      return;
    }
    uVar1 = param_2[0x17];
  }
  if (((((uVar1 != 0x418c04) && (uVar1 != 0x211780)) && (uVar1 != 0x214612)) &&
      (((uVar1 != 0x200801 && (uVar1 != 0x20079c)) &&
       ((uVar1 != 0x20079d && ((uVar1 != 0x20061b && (uVar1 != 0x400c03)))))))) &&
     ((uVar1 != 0x4011040b && (((uVar1 == 0x118005 || (uVar1 != 0x300c02)) && (uVar1 != 0x40001e))))
     )) {
    return;
  }
  uVar2 = param_3 & 0x20000;
  if ((uVar2 == 0) && (uVar4 = *param_2, (uVar4 & 4) == 0)) {
    if (uVar1 == 0x4011040b) goto LAB_0001cea4;
LAB_0001cd38:
    if (uVar4 == 0) goto LAB_0001cdc4;
    if ((uVar4 & 0x10) != 0) {
      return;
    }
    if ((char)param_2[0x11] == '\0') {
      return;
    }
    if (uVar2 != 0) {
      if (((param_2[8] != 0x10000) || (param_2[9] != 0)) ||
         ((param_2[0xb] != 0 || (param_2[0xc] != 0x10000)))) {
        (*(code *)(undefined *)0x0)("Matrix is not supported.");
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
        (*(code *)0x2b00)();
        return;
      }
      if (-1 < (int)(uVar3 << 0xb)) goto LAB_0001ce7c;
      goto LAB_0001cdd0;
    }
    if (-1 < (int)(uVar3 << 0xb)) goto LAB_0001ce88;
    uVar3 = param_2[0x1c];
  }
  else {
    if ((uVar3 & 0x3000000) != 0) {
      return;
    }
    if (uVar1 != 0x4011040b) {
      uVar4 = *param_2;
      goto LAB_0001cd38;
    }
    if (uVar2 != 0) {
      return;
    }
    uVar4 = *param_2;
LAB_0001cea4:
    if (uVar4 != 0) {
      return;
    }
LAB_0001cdc4:
    uVar4 = 0;
    if (-1 < (int)(uVar3 << 0xb)) {
      if (uVar2 == 0) goto LAB_0001ce88;
LAB_0001ce7c:
      if ((uVar3 & 0x7000) != 0) {
        return;
      }
      goto LAB_0001ce88;
    }
LAB_0001cdd0:
    uVar3 = param_2[0x1c];
  }
  if (((((((((uVar3 != 0x418c04) && (uVar3 == 0x211780)) || (uVar3 != 0x214612)) &&
          (uVar3 == 0x200801)) || (uVar3 != 0x20079c)) && (uVar3 == 0x20079d)) ||
       (uVar3 != 0x20061b)) && (uVar3 == 0x400c03)) || (uVar3 != 0x4011040b)) {
    if (uVar3 != 0x118005) {
      return;
    }
    if (uVar2 == 0) {
      if ((uVar4 & 4) != 0) {
        return;
      }
LAB_0001ce88:
      param_2[2] = param_2[2] | param_3;
      return;
    }
  }
  return;
}



/* ===== FUNCTION hcge_process_matrix @ 0001ced8 ===== */

void hcge_process_matrix(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  *(undefined1 *)(param_1 + 0x48) = 0;
  if (param_2 != 0x10000) {
    if (param_2 == 0x20000) {
      *(undefined1 *)(param_1 + 0x48) = 1;
      if (*param_1 != 4) {
        param_1[0x4c] = 0;
        param_1[0x4d] = 0;
        param_1[0x4e] = 0;
        param_1[0x4f] = 0;
        param_1[0x50] = 0;
        param_1[0x51] = 0;
        param_1[0x54] = 0;
        param_1[0x55] = 0;
        param_1[0x4a] = 0;
        param_1[0x4b] = 0x3ff00000;
        param_1[0x52] = 0;
        param_1[0x53] = 0x3ff00000;
        return;
      }
      if ((((param_1[8] == 0x10000) && (param_1[9] == 0)) && (param_1[0xb] == 0)) &&
         (param_1[0xc] == 0x10000)) {
        param_1[0x4c] = 0;
        param_1[0x4d] = 0;
        param_1[0x4e] = 0;
        param_1[0x4f] = 0;
        param_1[0x46] = param_1[10] + 0x8000 >> 0x10;
        param_1[0x47] = param_1[0xd] + 0x8000 >> 0x10;
        param_1[0x50] = 0;
        param_1[0x4a] = 0;
        param_1[0x4b] = 0x3ff00000;
        param_1[0x51] = 0;
        param_1[0x52] = 0;
        param_1[0x53] = 0x3ff00000;
        param_1[0x54] = 0;
        param_1[0x55] = 0;
        return;
      }
    }
    return;
  }
  if (*param_1 != 4) {
    return;
  }
  iVar1 = param_1[9];
  if (param_1[8] == 0x10000) {
    iVar2 = param_1[0xb];
    if (iVar1 == 0) {
      iVar3 = param_1[0xc];
      if ((iVar2 == 0) && (iVar1 = 0, iVar3 == 0x10000)) {
        param_1[0x46] = param_1[10] + 0x8000 >> 0x10;
        param_1[0x47] = param_1[0xd] + 0x8000 >> 0x10;
        return;
      }
      goto LAB_0001cf74;
    }
  }
  else {
    iVar2 = param_1[0xb];
  }
  iVar3 = param_1[0xc];
LAB_0001cf74:
  *(undefined1 *)(param_1 + 0x48) = 1;
  *(double *)(param_1 + 0x4e) = (double)((float)param_1[10] * 1.5258789e-05);
  *(double *)(param_1 + 0x50) = (double)((float)iVar2 * 1.5258789e-05);
  *(double *)(param_1 + 0x4c) = (double)((float)iVar1 * 1.5258789e-05);
  *(double *)(param_1 + 0x52) = (double)((float)iVar3 * 1.5258789e-05);
  *(double *)(param_1 + 0x4a) = (double)((float)param_1[8] * 1.5258789e-05);
  *(double *)(param_1 + 0x54) = (double)((float)param_1[0xd] * 1.5258789e-05);
  return;
}



/* ===== FUNCTION hcge_set_state @ 0001d0c4 ===== */

void hcge_set_state(int *param_1,int param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint *puVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint *puVar12;
  int iVar13;
  
  puVar12 = (uint *)param_1[0x56];
  iVar13 = *(int *)(param_2 + 0x48);
  memset(puVar12,0,0x2a8);
  if (iVar13 == 0x418c04) {
    iVar2 = 0;
LAB_0001d640:
    uVar9 = *(uint *)(hcge_color_table + iVar2 * 0x14 + 4);
    uVar4 = *(uint *)(hcge_color_table + iVar2 * 0x14 + 0x10);
    uVar8 = uVar9 & 0x1f;
  }
  else {
    if (iVar13 == 0x211780) {
      iVar2 = 1;
      goto LAB_0001d640;
    }
    iVar2 = 2;
    if (iVar13 == 0x214612) goto LAB_0001d640;
    if (iVar13 == 0x200801) {
      iVar2 = 3;
      goto LAB_0001d640;
    }
    if (iVar13 == 0x20079c) {
      iVar2 = 4;
      goto LAB_0001d640;
    }
    if ((iVar13 != 0x20079d) && (iVar2 = 6, iVar13 == 0x20061b)) goto LAB_0001d640;
    if (iVar13 == 0x400c03) {
      iVar2 = 7;
      goto LAB_0001d640;
    }
    puts("NOT a supported color format ");
    uVar8 = 0;
    uVar4 = 0;
    uVar9 = 0;
  }
  uVar5 = *(uint *)(param_2 + 0xb0);
  uVar10 = *puVar12;
  uVar6 = uVar5 & 0xf0000000;
  uVar3 = uVar10 | 2;
  *puVar12 = uVar3;
  if (uVar6 != 0) {
    puVar12[3] = uVar5;
  }
  puVar12[2] = uVar5 & 0xfffffff | (uint)(uVar6 != 0) << 0x1c;
  if (uVar4 == 0) {
    trap(7);
  }
  uVar4 = *(uint *)(param_2 + 0xb4) / uVar4 & 0xfff;
  puVar12[4] = puVar12[4] & 0xfffe0000 | uVar4 | uVar8 << 0xc;
  iVar13 = 0;
  if ((*(char *)(param_2 + 0xdc) != '\0') && (uVar9 - 2 < 5)) {
    iVar13 = 1;
    puVar12[0x1f] = 0xa000a4;
    uVar3 = uVar10 | 0x11002;
    puVar12[0x20] = 0xa000a4;
  }
  *puVar12 = uVar3 | 4;
  puVar12[1] = puVar12[1] & 0xbfffffff | iVar13 << 0x1e;
  if (uVar6 == 0) {
    puVar12[9] = puVar12[9] & 0xfffe0000 | uVar4 | uVar8 << 0xc;
    puVar12[7] = uVar5 & 0xfffffff;
    *(byte *)((int)param_1 + 0x10d) = (byte)((uint)(*(int *)(param_2 + 200) << 0x1d) >> 0x1f);
    if (param_3 == 0x10000) goto LAB_0001d45c;
LAB_0001d268:
    if (param_3 < 0x10001) {
      if (param_3 - 1 < 2) {
        puVar12 = (uint *)param_1[0x56];
        uVar4 = param_1[1];
        uVar8 = *puVar12;
        uVar9 = uVar8 | 0x8001;
        *puVar12 = uVar9;
        puVar12[1] = puVar12[1] & 0xff1ffff8 | 0xa00001;
        if ((uVar4 & 2) != 0) {
          uVar6 = puVar12[0x19];
          uVar9 = uVar8 | 0x8081;
          *puVar12 = uVar9;
          puVar12[0x19] = uVar6 & 0xffcfffff | 0x100000;
          puVar12[0x1a] = param_1[0x22];
          puVar12[0x19] = uVar6 & 0xf0cfffff | 0xe100000;
        }
        uVar3 = (uint)*(byte *)((int)param_1 + 0x85);
        uVar6 = (uint)*(byte *)((int)param_1 + 0x86);
        uVar8 = (uint)*(byte *)((int)param_1 + 0x87);
        if ((uVar4 & 4) != 0) {
          iVar13 = *(byte *)(param_1 + 0x21) + 1;
          uVar3 = (int)(uVar3 * iVar13) >> 8;
          uVar6 = (int)(uVar6 * iVar13) >> 8;
          uVar8 = (int)(uVar8 * iVar13) >> 8;
        }
        uVar5 = (uint)*(byte *)(param_1 + 0x21) << 0x18;
        uVar3 = uVar5 | uVar8 | uVar3 << 0x10;
        uVar6 = uVar3 | uVar6 << 8;
        param_1[0x45] = uVar6;
        *puVar12 = uVar9 | 0x20;
        if ((uVar4 & 8) != 0) {
          puVar12[0x19] = puVar12[0x19] & 0xffffdfff | 0x2000;
        }
        if ((uVar4 & 1) == 0) {
          uVar10 = uVar6 << 8;
          uVar11 = (puVar12[4] << 0xf) >> 0x1b;
          puVar12[0x19] = puVar12[0x19] & 0xfffcffff | 0x30000;
          uVar8 = uVar8 & 0xff;
          puVar12[1] = puVar12[1] & 0xfffff8c0 | 3;
          uVar1 = uVar6 << 0x10;
          switch(uVar11) {
          case 2:
            uVar6 = uVar1 >> 0x18 & 0xf0 | uVar8 >> 4 | (uVar10 >> 0x1c) << 8;
            break;
          case 3:
            uVar6 = (uVar5 >> 0x1c) << 0xc | uVar1 >> 0x18 & 0xf0 | uVar8 >> 4 |
                    (uVar10 >> 0x1c) << 8;
            break;
          case 4:
            uVar6 = (uVar10 >> 0x1b) << 10 | (uVar1 >> 0x1b) << 5 | uVar8 >> 3;
            break;
          case 5:
            uVar6 = (uVar3 >> 0x18 & 1) << 0xf | uVar8 >> 3 | (uVar10 >> 0x1b) << 10 |
                    (uVar1 >> 0x1b) << 5;
            break;
          case 6:
            uVar6 = (uVar10 >> 0x1b) << 0xb | uVar8 >> 3 | 0x10000 | (uVar1 >> 0x1a) << 5;
          }
          param_1[0x45] = uVar6;
          *puVar12 = uVar9 | 0x60;
          puVar12[0x21] = puVar12[0x21] & 0xffffffe0 | uVar11;
        }
        else {
          uVar8 = puVar12[0x19];
          puVar12[0x19] = uVar8 & 0xfffc3fff | 0x8000;
          puVar12[1] = puVar12[1] & 0xfffff8ff | 0x300;
          uVar6 = param_1[0x29];
          puVar12[0x19] = uVar8 & 0xfffc3ff0 | 0x8000 | uVar6 & 0xf;
          uVar3 = param_1[0x28];
          *puVar12 = uVar9 | 0x60;
          puVar12[0x21] = puVar12[0x21] & 0xffffffe0 | 1;
          puVar12[0x19] = uVar8 & 0xfffc3f00 | 0x8000 | uVar6 & 0xf | (uVar3 & 0xf) << 4;
        }
        if ((uVar4 & 0x10) != 0) {
          puVar12[0x19] = puVar12[0x19] & 0xfffbffff | 0x40000;
        }
        if ((uVar4 & 0x20) != 0) {
          puVar12[0x19] = puVar12[0x19] & 0xfff7ffff | 0x80000;
        }
        param_1[4] = 1;
      }
LAB_0001d3f8:
      *(undefined4 *)(param_2 + 200) = 0;
      return;
    }
    if (param_3 != 0x20000) goto LAB_0001d3f8;
    uVar4 = param_1[3];
    iVar13 = param_1[0x17];
    puVar12 = (uint *)param_1[0x56];
    if (uVar4 == 0) goto LAB_0001d6d8;
LAB_0001d46c:
    *(undefined1 *)(param_1 + 0x43) = 0;
    if ((uVar4 & 0xf000) == 0) {
LAB_0001d6e4:
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
    }
    else if ((uVar4 & 0x2000) == 0) {
      if ((uVar4 & 0x1000) == 0) {
        if ((uVar4 & 0x4000) != 0) {
          param_1[0x10a] = 0;
          param_1[0x10b] = 0x4070e000;
        }
      }
      else {
        param_1[0x10a] = 0;
        param_1[0x10b] = 0x40668000;
      }
    }
    else {
      param_1[0x10a] = 0;
      param_1[0x10b] = 0x40568000;
    }
  }
  else {
    puVar12[8] = uVar5;
    puVar12[9] = puVar12[9] & 0xfffe0000 | uVar4 | uVar8 << 0xc;
    puVar12[7] = uVar5 & 0xfffffff | 0x20000000;
    *(byte *)((int)param_1 + 0x10d) = (byte)((uint)(*(int *)(param_2 + 200) << 0x1d) >> 0x1f);
    if (param_3 != 0x10000) goto LAB_0001d268;
LAB_0001d45c:
    uVar4 = param_1[3];
    iVar13 = param_1[0x17];
    puVar12 = (uint *)param_1[0x56];
    if (uVar4 != 0) goto LAB_0001d46c;
LAB_0001d6d8:
    if (((param_3 != 0x10000) || (iVar13 != param_1[0x12])) || (*param_1 != 0)) {
      *(undefined1 *)(param_1 + 0x43) = 0;
      goto LAB_0001d6e4;
    }
    param_1[0x10a] = 0;
    param_1[0x10b] = 0;
    *(undefined1 *)(param_1 + 0x43) = 1;
  }
  hcge_process_matrix(param_1,param_3);
  if (iVar13 == 0x418c04) {
    iVar2 = 0;
LAB_0001db9c:
    iVar2 = iVar2 * 0x14;
    uVar3 = *(uint *)(hcge_color_table + iVar2 + 0xc);
    uVar8 = *(uint *)(hcge_color_table + iVar2 + 0x10);
    uVar6 = (uint)(*(uint *)(hcge_color_table + iVar2 + 4) == 0x1e);
    uVar9 = *(uint *)(hcge_color_table + iVar2 + 4) & 0x1f;
  }
  else {
    if (iVar13 == 0x211780) {
      iVar2 = 1;
      goto LAB_0001db9c;
    }
    iVar2 = 2;
    if (iVar13 == 0x214612) goto LAB_0001db9c;
    if (iVar13 == 0x200801) {
      iVar2 = 3;
      goto LAB_0001db9c;
    }
    if (iVar13 == 0x20079c) {
      iVar2 = 4;
      goto LAB_0001db9c;
    }
    if (iVar13 == 0x20079d) {
      iVar2 = 5;
      goto LAB_0001db9c;
    }
    iVar2 = 6;
    if ((((iVar13 == 0x20061b) || (iVar2 = 7, iVar13 == 0x400c03)) ||
        ((iVar2 = 8, iVar13 == 0x4011040b ||
         ((iVar13 != 0x118005 && (iVar2 = 10, iVar13 == 0x300c02)))))) ||
       (iVar2 = 0xb, iVar13 == 0x40001e)) goto LAB_0001db9c;
    puts("NOT a supported color format ");
    uVar9 = 0;
    uVar6 = 0;
    uVar8 = 0;
    uVar3 = 0;
  }
  uVar5 = puVar12[1];
  uVar10 = *puVar12;
  puVar12[1] = uVar5 & 0x7fffffff | uVar6 << 0x1f;
  *puVar12 = uVar10 | 1;
  if ((char)param_1[0x43] != '\0') {
    uVar10 = uVar10 | 5;
    *puVar12 = uVar10;
    puVar12[1] = uVar5 & 0x7ffffff8 | uVar6 << 0x1f | 2;
    uVar6 = param_1[0x2e];
    if ((uVar6 & 0xf0000000) == 0) {
      iVar13 = 0;
    }
    else {
      iVar13 = 3;
      puVar12[8] = uVar6;
    }
    puVar12[7] = uVar6 & 0xfffffff | iVar13 << 0x1c;
    if (uVar8 == 0) {
      trap(7);
    }
    puVar12[9] = puVar12[9] & 0xffce0000 | (uint)param_1[0x2f] / uVar8 & 0xfff | uVar9 << 0xc |
                 ((uVar4 << 7) >> 0x1f) << 0x14 | ((uVar4 << 6) >> 0x1f) << 0x15;
    uVar4 = param_1[3];
    goto LAB_0001d5f8;
  }
  uVar6 = param_1[0x2e];
  *puVar12 = uVar10 | 9;
  if ((uVar6 & 0xf0000000) == 0) {
    iVar13 = 0;
  }
  else {
    puVar12[0xc] = uVar6;
    iVar13 = 3;
  }
  puVar12[0xb] = uVar6 & 0xfffffff | iVar13 << 0x1c;
  if ((int)puVar12[1] < 0) {
    iVar13 = param_1[0x2f];
    uVar8 = (iVar13 + 3U) * 0x40000 >> 0x14;
    puVar12[7] = puVar12[7] & 0xf0000000 | uVar6 & 0xfffffff;
    puVar12[9] = puVar12[9] & 0xfffff000 | uVar8;
    puVar12[4] = puVar12[4] & 0xfffff000 | (iVar13 + 3U & 0xfffffffc) / 3 & 0xfff;
  }
  else {
    uVar6 = param_1[0x2f];
    if (uVar8 == 0) {
      trap(7);
    }
    puVar12[0xd] = puVar12[0xd] & 0xfff9ffff | (uVar3 & 3) << 0x11;
    uVar8 = uVar6 / uVar8 & 0xfff;
  }
  uVar3 = puVar12[1];
  uVar6 = (~(uVar3 & 0xfffffff8) >> 0x1f) << 3;
  uVar5 = uVar3 & 0xffffffc0 | 1 | uVar6;
  puVar12[0xd] = puVar12[0xd] & 0xffce0000 | uVar8 | uVar9 << 0xc | ((uVar4 << 7) >> 0x1f) << 0x14 |
                 ((uVar4 << 6) >> 0x1f) << 0x15;
  puVar12[1] = uVar5;
  if (param_1[0x26] != 1) {
    iVar13 = 5;
    if (param_1[0x26] == 3) {
      puVar12[1] = uVar3 & 0xff1fffc0 | 1 | uVar6 | 0x200000;
      uVar4 = param_1[3];
    }
    else {
      if ((int)uVar5 < 0) {
        iVar13 = 0;
      }
      puVar12[1] = uVar3 & 0xff1fffc0 | 1 | uVar6 | iVar13 << 0x15;
      uVar4 = param_1[3];
      uVar8 = param_1[0x28];
      uVar9 = param_1[0x29];
      puVar7 = (uint *)param_1[0x56];
      uVar6 = uVar4 & 3;
      iVar13 = 0;
      if (((int)(uVar4 << 0xb) < 0) && (iVar13 = 1, uVar6 == 0)) {
        uVar6 = 1;
        uVar8 = 2;
        uVar9 = 1;
        uVar4 = uVar4 | 1;
      }
      uVar3 = *puVar7;
      uVar5 = uVar3 | 0x20000;
      *puVar7 = uVar5;
      uVar10 = param_1[0x21];
      uVar10 = (((uVar10 >> 0x10) << 0x18 | (uVar10 >> 0x18) << 0x10) >> 0x10) +
               ((uVar10 & 0xff) << 8 | uVar10 >> 8 & 0xff) * 0x10000;
      puVar7[0x22] = uVar10;
      if ((uVar4 & 0x10) != 0) {
        uVar11 = puVar7[0x19] & 0xffcfffff | 0x100000;
        uVar5 = uVar3 | 0x20080;
        puVar7[0x19] = uVar11;
        *puVar7 = uVar5;
        uVar3 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x22]);
        puVar7[0x1a] = uVar3;
        puVar7[0x19] = uVar11 & 0xf0ffffff | 0xe000000;
      }
      if ((uVar4 & 8) != 0) {
        uVar5 = uVar5 | 0x80;
        uVar11 = puVar7[0x19] & 0xff3fffff | 0x800000;
        *puVar7 = uVar5;
        puVar7[0x19] = uVar11;
        uVar3 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x23]);
        puVar7[0x1b] = uVar3;
        puVar7[0x19] = uVar11 & 0xfffffff | 0xe0000000;
      }
      if ((int)(uVar4 << 2) < 0) {
        uVar3 = param_1[0x2b];
        *puVar7 = uVar5 | 0x80;
        if (uVar3 == 0) {
          puVar7[0x19] = puVar7[0x19] & 0xffcfffff | 0x100000;
          uVar3 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x22]);
          puVar7[0x1a] = uVar3;
LAB_0001ddec:
          puVar7[0x19] = puVar7[0x19] & 0xf0ffffff | 0xe000000;
        }
        else {
          if ((uVar3 == 3) || (uVar3 == 5)) {
            puVar7[0x19] = puVar7[0x19] & 0xffcfffff | 0x100000;
            uVar5 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x22]);
            puVar7[0x1a] = uVar5;
          }
          else {
            puVar7[0x19] = puVar7[0x19] & 0xffcfffff | 0x200000;
            uVar5 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x22]);
            puVar7[0x1a] = uVar5;
            if (uVar3 < 2) goto LAB_0001ddec;
          }
          if (uVar3 - 2 < 2) {
            puVar7[0x19] = puVar7[0x19] & 0xf0ffffff | 0x1000000;
          }
          else if (uVar3 - 4 < 2) goto LAB_0001ddec;
        }
      }
      if ((int)(uVar4 << 1) < 0) {
        uVar3 = param_1[0x2a];
        *puVar7 = *puVar7 | 0x80;
        if (uVar3 == 0) {
          puVar7[0x19] = puVar7[0x19] & 0xff3fffff | 0x400000;
          uVar3 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x23]);
          puVar7[0x1b] = uVar3;
LAB_0001de6c:
          puVar7[0x19] = puVar7[0x19] & 0xfffffff | 0xe0000000;
        }
        else {
          if ((uVar3 == 3) || (uVar3 == 5)) {
            puVar7[0x19] = puVar7[0x19] & 0xff3fffff | 0x400000;
            uVar5 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x23]);
            puVar7[0x1b] = uVar5;
          }
          else {
            puVar7[0x19] = puVar7[0x19] & 0xff3fffff | 0x800000;
            uVar5 = to_argb8888((puVar7[0xd] << 0xf) >> 0x1b,param_1[0x23]);
            puVar7[0x1b] = uVar5;
            if (uVar3 < 2) goto LAB_0001de6c;
          }
          if (uVar3 - 2 < 2) {
            puVar7[0x19] = puVar7[0x19] & 0xfffffff | 0x10000000;
          }
          else if (uVar3 - 4 < 2) goto LAB_0001de6c;
        }
      }
      if ((uVar4 & 4) == 0) {
        if ((uVar4 & 2) == 0) {
          uVar3 = puVar7[0x19];
          if (iVar13 != 0) {
            iVar13 = 2;
            goto LAB_0001dc50;
          }
          uVar3 = uVar3 & 0xfffff1ff;
          puVar7[0x19] = uVar3;
        }
        else if ((uVar4 & 1) == 0) {
          uVar3 = puVar7[0x19] & 0xfffff1ff | 0x200;
          puVar7[0x19] = uVar3;
        }
        else {
          uVar3 = puVar7[0x19] & 0xfffff1ff | 0x400;
          puVar7[0x19] = uVar3;
        }
      }
      else if ((uVar4 & 2) == 0) {
        uVar3 = puVar7[0x19];
        if (iVar13 == 0) {
          uVar3 = uVar3 & 0xfffff1ff | 0x600;
          puVar7[0x19] = uVar3;
        }
        else {
          iVar13 = 5;
LAB_0001dc50:
          uVar3 = uVar3 & 0xfffff1ff | iVar13 << 9;
          puVar7[0x19] = uVar3;
          puVar7[0x22] = uVar10 | 0xff000000;
        }
      }
      else if ((uVar4 & 1) == 0) {
        uVar3 = puVar7[0x19] & 0xfffff1ff | 0x800;
        puVar7[0x19] = uVar3;
      }
      else {
        uVar3 = puVar7[0x19] & 0xfffff1ff | 0xa00;
        puVar7[0x19] = uVar3;
      }
      if ((uVar4 & 0x20) != 0) {
        uVar3 = uVar3 & 0xffffefff | 0x1000;
        puVar7[0x19] = uVar3;
      }
      if ((uVar4 & 0x200) != 0) {
        uVar3 = uVar3 & 0xfffffeff | 0x100;
        puVar7[0x19] = uVar3;
      }
      if ((uVar4 & 0x40) != 0) {
        uVar3 = uVar3 & 0xffffdfff | 0x2000;
        puVar7[0x19] = uVar3;
      }
      iVar13 = 1;
      if (uVar6 != 0) {
        iVar13 = 2;
        uVar3 = puVar7[0x19] & 0xffffff00 | uVar9 & 0xf | (uVar8 & 0xf) << 4;
        puVar7[0x19] = uVar3;
      }
      puVar7[0x19] = uVar3 & 0xffff3fff | iVar13 << 0xe;
      if ((uVar4 & 0x80) != 0) {
        puVar7[0x19] = uVar3 & 0xfffb3fff | iVar13 << 0xe | 0x40000;
      }
      if ((uVar4 & 0x400) != 0) {
        puVar7[0x19] = puVar7[0x19] & 0xfff7ffff | 0x80000;
      }
      uVar4 = param_1[3];
    }
    goto LAB_0001dd00;
  }
  puVar12[1] = uVar3 & 0xff1fffc0 | 1 | uVar6 | 0x400000;
  uVar4 = param_1[3];
  puVar7 = (uint *)param_1[0x56];
  if ((uVar4 & 0x60000000) == 0) goto LAB_0001dfc4;
  uVar8 = puVar7[0x19];
  puVar7[0x19] = uVar8 & 0x3fffffff;
  *puVar7 = *puVar7 | 0x80;
  switch(param_1[0x2b]) {
  case 0:
    puVar7[0x19] = uVar8 & 0xffffff | 0x15000000;
    break;
  case 1:
    puVar7[0x19] = uVar8 & 0xffffff | 0x2a000000;
    break;
  case 2:
    puVar7[0x19] = puVar7[0x19] & 0xff3fffff | 0x400000;
    break;
  case 3:
    puVar7[0x19] = puVar7[0x19] & 0xff3fffff | 0x800000;
    break;
  case 4:
    iVar13 = 1;
    goto LAB_0001df7c;
  case 5:
    iVar13 = 2;
LAB_0001df7c:
    puVar7[0x19] = uVar8 & 0x3fffff | iVar13 << 0x1c | iVar13 << 0x1a | iVar13 << 0x18 |
                   iVar13 << 0x16;
  }
  uVar8 = (puVar7[0xd] << 0xf) >> 0x1b;
  uVar9 = to_argb8888(uVar8,param_1[0x25]);
  iVar13 = param_1[0x24];
  puVar7[0x1a] = uVar9;
  uVar8 = to_argb8888(uVar8,iVar13);
  puVar7[0x1b] = uVar8;
LAB_0001dfc4:
  if ((uVar4 & 0x20) == 0) {
    uVar8 = puVar7[0x19];
  }
  else {
    uVar8 = puVar7[0x19] | 0x1000;
    puVar7[0x19] = uVar8;
  }
  puVar7[0x19] = uVar8 & 0xffffdfff | (((uVar4 ^ 0x40) << 0x19) >> 0x1f) << 0xd;
  *(char *)(puVar7 + 0x19) = (char)param_1[0x21];
  if ((uVar4 & 3) != 0) {
    puVar7[0x19] = puVar7[0x19] & 0xfffff0ff | (param_1[0x27] & 0xfU) << 8;
  }
LAB_0001dd00:
  uVar10 = *puVar12 | 0x8000;
  *puVar12 = uVar10;
LAB_0001d5f8:
  if ((int)(uVar4 << 0xb) < 0) {
    iVar13 = param_1[0x1c];
    if ((((((((((iVar13 == 0x418c04) || (iVar13 != 0x211780)) && (iVar13 == 0x214612)) ||
             (iVar13 != 0x200801)) && (iVar13 == 0x20079c)) || (iVar13 != 0x20079d)) &&
          (iVar13 == 0x20061b)) || (iVar13 != 0x400c03)) && (iVar13 == 0x4011040b)) ||
       (iVar13 != 0x118005)) {
      puts("NOT a supported color format ");
      uVar10 = *puVar12;
    }
    *puVar12 = uVar10 | 0x10;
    puVar12[1] = puVar12[1] & 0xffffff3f | 0x40;
    uVar4 = param_1[0x30];
    if ((uVar4 & 0xf0000000) == 0) {
      iVar13 = 0;
    }
    else {
      iVar13 = 4;
      puVar12[0x11] = uVar4;
    }
    puVar12[0x10] = uVar4 & 0xfffffff | iVar13 << 0x1c;
    puVar12[0x12] = puVar12[0x12] & 0xfffe0000 | param_1[0x31] & 0xfffU | 0x1d000;
    param_1[4] = param_3;
    *(undefined1 *)((int)param_1 + 0x10f) = 1;
  }
  else {
    *(undefined1 *)((int)param_1 + 0x10f) = 0;
    param_1[4] = param_3;
  }
  *(undefined4 *)(param_2 + 200) = 0;
  return;
}



/* ===== FUNCTION hcge_check_state @ 0001e14c ===== */

void hcge_check_state(uint *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  if (((param_2 & 0x30001) == 0) ||
     (((((uVar3 = param_1[0x12], uVar3 != 0x418c04 && (uVar3 != 0x211780)) && (uVar3 != 0x214612))
       && (((uVar3 != 0x200801 && (uVar3 != 0x20079c)) &&
           ((uVar3 == 0x20079d || (uVar3 != 0x20061b)))))) && (uVar3 != 0x400c03)))) {
    return;
  }
  if ((param_2 & 0xffff) != 0) {
    if (((*param_1 == 0) || (*param_1 == 3)) &&
       (((param_2 & 1) != 0 && ((param_1[1] == 0 || ((param_1[1] & 0xaf) != 0)))))) {
      param_1[2] = param_1[2] | param_2;
      return;
    }
    return;
  }
  if ((param_2 & 0x30000) == 0) {
    return;
  }
  uVar3 = param_1[3];
  if (uVar3 == 0) {
    uVar2 = param_1[0x17];
  }
  else {
    if ((uVar3 & 0x31176ff) == 0) {
      return;
    }
    uVar2 = param_1[0x17];
  }
  if ((((((uVar2 != 0x418c04) && (uVar2 != 0x211780)) && (uVar2 != 0x214612)) &&
       (((uVar2 != 0x200801 && (uVar2 != 0x20079c)) &&
        ((uVar2 != 0x20079d && ((uVar2 != 0x20061b && (uVar2 != 0x400c03)))))))) &&
      (uVar2 != 0x4011040b)) &&
     (((uVar2 == 0x118005 || (uVar2 != 0x300c02)) && (uVar2 != 0x40001e)))) {
    return;
  }
  uVar1 = param_2 & 0x20000;
  if ((uVar1 == 0) && (uVar4 = *param_1, (uVar4 & 4) == 0)) {
    if (uVar2 == 0x4011040b) goto LAB_0001cea4;
LAB_0001cd38:
    if (uVar4 == 0) goto LAB_0001cdc4;
    if ((uVar4 & 0x10) != 0) {
      return;
    }
    if ((char)param_1[0x11] == '\0') {
      return;
    }
    if (uVar1 != 0) {
      if (((param_1[8] != 0x10000) || (param_1[9] != 0)) ||
         ((param_1[0xb] != 0 || (param_1[0xc] != 0x10000)))) {
        (*(code *)(undefined *)0x0)("Matrix is not supported.");
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
        (*(code *)0x2b00)();
        return;
      }
      if (-1 < (int)(uVar3 << 0xb)) goto LAB_0001ce7c;
      goto LAB_0001cdd0;
    }
    if (-1 < (int)(uVar3 << 0xb)) goto LAB_0001ce88;
    uVar3 = param_1[0x1c];
  }
  else {
    if ((uVar3 & 0x3000000) != 0) {
      return;
    }
    if (uVar2 != 0x4011040b) {
      uVar4 = *param_1;
      goto LAB_0001cd38;
    }
    if (uVar1 != 0) {
      return;
    }
    uVar4 = *param_1;
LAB_0001cea4:
    if (uVar4 != 0) {
      return;
    }
LAB_0001cdc4:
    uVar4 = 0;
    if (-1 < (int)(uVar3 << 0xb)) {
      if (uVar1 == 0) goto LAB_0001ce88;
LAB_0001ce7c:
      if ((uVar3 & 0x7000) != 0) {
        return;
      }
      goto LAB_0001ce88;
    }
LAB_0001cdd0:
    uVar3 = param_1[0x1c];
  }
  if (((((((((uVar3 != 0x418c04) && (uVar3 == 0x211780)) || (uVar3 != 0x214612)) &&
          (uVar3 == 0x200801)) || (uVar3 != 0x20079c)) && (uVar3 == 0x20079d)) ||
       (uVar3 != 0x20061b)) && (uVar3 == 0x400c03)) || (uVar3 != 0x4011040b)) {
    if (uVar3 != 0x118005) {
      return;
    }
    if (uVar1 == 0) {
      if ((uVar4 & 4) != 0) {
        return;
      }
LAB_0001ce88:
      param_1[2] = param_1[2] | param_2;
      return;
    }
  }
  return;
}



/* ===== FUNCTION hcge_state_init @ 0001e238 ===== */

void hcge_state_init(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    memset(param_1,0,0xe0);
    *param_1 = 0;
    *(undefined1 *)(param_1 + 0x21) = 0xff;
    param_1[3] = 0;
    param_1[0x26] = 1;
    param_1[0x27] = 0;
    param_1[0x29] = 1;
    param_1[0x28] = 5;
  }
  return;
}



/* ===== FUNCTION hcge_blend_to_string @ 0001e2b0 ===== */

char * hcge_blend_to_string(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if ((((((param_1 != 1) && (iVar1 = 1, param_1 != 2)) && (iVar1 = 2, param_1 != 3)) &&
       ((iVar1 = 3, param_1 != 4 && (iVar1 = 4, param_1 != 5)))) &&
      ((iVar1 = 5, param_1 != 6 && ((iVar1 = 6, param_1 != 7 && (iVar1 = 7, param_1 != 8)))))) &&
     ((iVar1 = 8, param_1 != 9 && (iVar1 = 9, param_1 != 10)))) {
    if (param_1 != 0xb) {
      iVar1 = 0x58;
      if (param_1 != 0) {
        return "<unknown>";
      }
      goto LAB_0001e35c;
    }
    iVar1 = 10;
  }
  iVar1 = iVar1 << 3;
LAB_0001e35c:
  return *(char **)(m_bfuncs + iVar1 + 4);
}



/* ===== FUNCTION hcge_drawingflags_to_string @ 0001e398 ===== */

undefined1 * hcge_drawingflags_to_string(uint param_1)

{
  bool bVar1;
  size_t sVar2;
  uint *puVar3;
  size_t sVar4;
  char *pcVar5;
  
  if (param_1 == 0) {
    return "NOFX";
  }
  sVar4 = 0;
  bVar1 = false;
  puVar3 = &m_dflags;
  do {
    if ((param_1 & *puVar3) != 0) {
      if (bVar1) {
        pcVar5 = (char *)puVar3[1];
        snprintf(s_dflags_name_3894 + sVar4,0x200," | %s",pcVar5);
        sVar2 = strlen(pcVar5);
        sVar4 = sVar4 + 3 + sVar2;
      }
      else {
        pcVar5 = (char *)puVar3[1];
        bVar1 = true;
        sVar4 = strlen(pcVar5);
        snprintf(s_dflags_name_3894,0x200,"%s",pcVar5);
      }
    }
    puVar3 = puVar3 + 2;
  } while (puVar3 != (uint *)m_bfuncs);
  if (bVar1) {
    s_dflags_name_3894[sVar4] = 0;
    return s_dflags_name_3894;
  }
  return "<unknown>";
}



/* ===== FUNCTION hcge_blittingflags_to_string @ 0001e558 ===== */

undefined1 * hcge_blittingflags_to_string(uint param_1)

{
  bool bVar1;
  size_t sVar2;
  undefined **ppuVar3;
  size_t sVar4;
  char *pcVar5;
  
  if (param_1 == 0) {
    return "NOFX";
  }
  sVar4 = 0;
  bVar1 = false;
  ppuVar3 = &PTR_s_BLEND_ALPHACHANNEL_00033108;
  do {
    if ((param_1 & (uint)ppuVar3[-1]) != 0) {
      if (bVar1) {
        pcVar5 = *ppuVar3;
        snprintf(s_bflags_name_3904 + sVar4,0x200," | %s",pcVar5);
        sVar2 = strlen(pcVar5);
        sVar4 = sVar4 + 3 + sVar2;
      }
      else {
        pcVar5 = *ppuVar3;
        bVar1 = true;
        sVar4 = strlen(pcVar5);
        snprintf(s_bflags_name_3904,0x200,"%s",pcVar5);
      }
    }
    ppuVar3 = ppuVar3 + 2;
  } while (ppuVar3 != &PTR_s_BLEND_000331d8);
  if (bVar1) {
    s_bflags_name_3904[sVar4] = 0;
    return s_bflags_name_3904;
  }
  return "<unknown>";
}



/* ===== FUNCTION hcge_state_modify_flags @ 0001e718 ===== */

undefined1 * hcge_state_modify_flags(uint param_1)

{
  bool bVar1;
  size_t sVar2;
  undefined **ppuVar3;
  undefined **ppuVar4;
  int iVar5;
  char *__s;
  
  if (param_1 == 0) {
    return "NONE";
  }
  iVar5 = 0;
  bVar1 = false;
  ppuVar4 = &PTR_s_BLEND_ALPHACHANNEL_00033108;
  ppuVar3 = &PTR_s_SMF_DRAWING_FLAGS_00033080;
  do {
    if ((param_1 & (uint)ppuVar3[-1]) != 0) {
      if (bVar1) {
        __s = *ppuVar3;
        snprintf(s_bflags_name_3948 + iVar5,0x200," | %s",__s);
        sVar2 = strlen(__s);
        iVar5 = iVar5 + 3 + sVar2;
      }
      else {
        bVar1 = true;
        sVar2 = strlen(*ppuVar3);
        iVar5 = iVar5 + sVar2;
        snprintf(s_bflags_name_3948,0x200,"%s",*ppuVar4);
      }
    }
    ppuVar4 = ppuVar4 + 2;
    ppuVar3 = ppuVar3 + 2;
  } while (ppuVar3 != &PTR_s_BLEND_ALPHACHANNEL_00033108);
  if (bVar1) {
    s_bflags_name_3948[iVar5] = 0;
    return s_bflags_name_3948;
  }
  return "<unknown>";
}



/* ===== FUNCTION hcge_pixelformat_name @ 0001e8e8 ===== */

char * hcge_pixelformat_name(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_1 != 0x211780) {
    if (param_1 == 0x200801) {
      iVar1 = 1;
    }
    else if (param_1 == 0x400c03) {
      iVar1 = 2;
    }
    else if (param_1 == 0x418c04) {
      iVar1 = 3;
    }
    else if (param_1 == 0x118005) {
      iVar1 = 4;
    }
    else if (param_1 == 0x4011040b) {
      iVar1 = 5;
    }
    else if (param_1 == 0x214612) {
      iVar1 = 6;
    }
    else if (param_1 == 0x20061b) {
      iVar1 = 7;
    }
    else if (param_1 == 0x20079c) {
      iVar1 = 8;
    }
    else {
      iVar1 = 9;
      if ((param_1 != 0x20079d) && (iVar1 = 10, param_1 != 0x300c02)) {
        if (param_1 == 0x40001e) {
          iVar1 = 0xb;
        }
        else {
          iVar1 = 0xc;
          if (param_1 != 0) {
            return "unknown pixelformat";
          }
        }
      }
    }
  }
  return *(char **)(hcge_pixelformat_names + iVar1 * 8 + 4);
}



/* ===== FUNCTION tunefilter @ 0001ea00 ===== */

void tunefilter(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  short sVar5;
  int iVar6;
  short *psVar7;
  short *psVar8;
  
  if (0 < param_1) {
    iVar2 = (param_2 >> 1) * param_1;
    psVar7 = (short *)(param_4 + iVar2 * 2);
    psVar8 = (short *)(param_4 + (iVar2 - param_1) * 2);
    do {
      psVar3 = psVar7 + -iVar2;
      iVar6 = 0;
      if (0 < param_2) {
        iVar4 = 0;
        do {
          sVar5 = *psVar3;
          iVar4 = iVar4 + 1;
          psVar3 = psVar3 + param_1;
          iVar6 = iVar6 + sVar5;
        } while (param_2 != iVar4);
      }
      if (param_3 == iVar6) {
LAB_0001eb08:
        iVar6 = (int)*psVar7;
      }
      else {
        sVar5 = *psVar7;
        sVar1 = *psVar8;
        iVar6 = param_3 - iVar6;
        if (sVar1 < sVar5) {
          sVar5 = sVar5 + (short)iVar6;
          iVar6 = (int)sVar5;
          *psVar7 = sVar5;
        }
        else {
          if (sVar1 <= sVar5) {
            *psVar7 = (short)((iVar6 + 1) / 2) + sVar5;
            *psVar8 = (short)(iVar6 / 2) + *psVar8;
            goto LAB_0001eb08;
          }
          *psVar8 = sVar1 + (short)iVar6;
          iVar6 = (int)*psVar7;
        }
      }
      sVar5 = (short)iVar6 - (short)param_3;
      if (param_3 < iVar6) {
        *psVar7 = (short)iVar6 - sVar5;
        *psVar8 = sVar5 + *psVar8;
      }
      else {
        sVar5 = *psVar8;
        sVar1 = sVar5 - (short)param_3;
        if (param_3 < sVar5) {
          *psVar8 = sVar5 - sVar1;
          *psVar7 = sVar1 + *psVar7;
        }
      }
      psVar7 = psVar7 + 1;
      psVar8 = psVar8 + 1;
    } while ((short *)(param_4 + iVar2 * 2 + param_1 * 2) != psVar7);
  }
  return;
}



/* ===== FUNCTION designfilter @ 0001eb2c ===== */

void designfilter(int param_1,int param_2,int param_3,int param_4,int param_5,undefined4 param_6,
                 double param_7,uint param_8,double *param_9,undefined2 *param_10)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 *puVar7;
  uint uVar8;
  undefined2 uVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  undefined4 local_158;
  uint uStack_154;
  double local_150;
  double local_148;
  undefined8 local_140;
  double local_138;
  double local_130;
  undefined4 local_128;
  undefined4 uStack_124;
  undefined4 local_120;
  undefined4 uStack_11c;
  double local_118;
  double local_110;
  double local_108;
  double local_100;
  double local_f8;
  double local_f0;
  double local_e8;
  double local_e0;
  double local_d8;
  double local_d0;
  double local_c8;
  double local_c0;
  double local_b8;
  double local_b0;
  double local_a8;
  uint local_a0;
  double *local_9c;
  undefined *local_98;
  undefined *local_94;
  undefined4 local_90;
  uint uStack_8c;
  double local_88;
  double *local_80;
  double *local_7c;
  undefined2 *local_78;
  uint local_74;
  int local_70;
  undefined *local_6c;
  double local_68;
  undefined *local_60;
  undefined *local_5c;
  
  local_78 = param_10;
  local_74 = param_8;
  uVar8 = param_3 * param_4 + param_5;
  if (param_2 < param_1) {
    local_98 = (undefined *)0x20000;
    local_a0 = uVar8 & 0x80000001;
    dVar10 = ((double)param_2 / (double)param_1) * (1.0 / (double)param_3);
  }
  else {
    local_98 = (undefined *)0x20000;
    local_a0 = uVar8 & 0x80000001;
    dVar10 = 1.0 / (double)param_3;
  }
  local_a0 = uVar8 & 0x80000001;
  if ((int)local_a0 < 0) {
    local_a0 = ((uVar8 & 0x80000001) - 1 | 0xfffffffe) + 1;
  }
  local_98 = (undefined *)0x20000;
  local_158 = 0;
  local_140 = 0x3fe0000000000000;
  iVar6 = (int)uVar8 / 2;
  uStack_154 = 0;
  local_128 = 0;
  uStack_124 = 0;
  local_120 = 0;
  uStack_11c = 0;
  local_94 = (undefined *)0x20000;
  local_138 = 1.0;
  dVar10 = dVar10 * param_7 * 0.5;
  local_150 = dVar10;
  local_148 = dVar10;
  local_130 = local_138;
  if (0 < (int)uVar8) {
    memset(param_9,0,uVar8 * 8);
  }
  pdVar1 = &local_138;
  local_118 = *(double *)(local_98 + 0x25c0);
  local_5c = (undefined *)0x20000;
  dVar14 = 0.0;
  local_70 = iVar6 * 8;
  local_6c = (undefined *)0x20000;
  local_e0 = local_118;
  local_60 = (undefined *)0x20000;
  local_d8 = local_118;
  dVar13 = dVar10 - 0.0;
  local_7c = param_9 + (uint)(local_a0 != 0) + iVar6;
  local_d0 = local_118;
  local_68 = 39.47841760435743;
  local_b8 = 3.141592653589793;
  local_80 = &local_118;
  local_b0 = 3.141592653589793;
  local_a8 = 3.141592653589793;
  local_f0 = *(double *)(local_94 + 0x2b70);
  dVar15 = (local_130 - local_138) / dVar13;
  local_88 = 0.0;
  local_e8 = 6.283185307179586;
  dVar12 = dVar15 * 0.0;
  dVar11 = local_138 - dVar12;
  local_9c = (double *)&local_158;
  if (local_a0 == 0) goto LAB_0001eeb4;
LAB_0001ed10:
  local_88 = local_88 +
             (dVar10 * dVar10 - dVar14 * dVar14) * dVar15 * *(double *)(local_94 + 0x2b70) +
             dVar13 * dVar11;
  if (0 < iVar6) {
    iVar5 = 0;
    local_f8 = *(double *)(local_60 + 0x2b80);
    local_c0 = *(double *)(local_5c + 0x2b88);
    local_100 = (dVar12 + dVar11) * dVar14;
    dVar12 = 0.0;
    local_110 = dVar15 / *(double *)(local_6c + 0x2b78);
    local_108 = (dVar15 * dVar10 + dVar11) * dVar10;
    pdVar2 = local_7c;
    do {
      dVar15 = (double)iVar5 + local_118;
      dVar13 = dVar15 * local_f8;
      dVar16 = dVar15 + dVar15;
      dVar11 = cos(dVar13 * dVar10);
      dVar13 = cos(dVar13 * dVar14);
      dVar13 = ((dVar11 - dVar13) * local_110) / (dVar15 * dVar15) + *pdVar2;
      dVar11 = local_118;
      if (dVar16 * dVar10 != dVar12) {
        dVar15 = dVar16 * dVar10 * local_c0;
        dVar11 = sin(dVar15);
        dVar11 = dVar11 / dVar15;
      }
      dVar11 = dVar11 * local_108;
      dVar15 = local_d0;
      if (dVar16 * dVar14 != dVar12) {
        dVar15 = dVar16 * dVar14 * local_a8;
        local_c8 = dVar11;
        dVar11 = sin(dVar15);
        dVar15 = dVar11 / dVar15;
        dVar11 = local_c8;
      }
      iVar5 = iVar5 + 1;
      *pdVar2 = (dVar11 - dVar15 * local_100) + dVar13;
      pdVar2 = pdVar2 + 1;
    } while (iVar6 != iVar5);
  }
  while( true ) {
    pdVar1 = pdVar1 + 2;
    local_9c = local_9c + 2;
    if (local_80 == pdVar1) break;
    while( true ) {
      dVar14 = *local_9c;
      dVar10 = local_9c[1];
      dVar13 = dVar10 - dVar14;
      dVar15 = (pdVar1[1] - *pdVar1) / dVar13;
      dVar12 = dVar15 * dVar14;
      dVar11 = *pdVar1 - dVar12;
      if (local_a0 != 0) goto LAB_0001ed10;
LAB_0001eeb4:
      if (iVar6 < 1) break;
      iVar5 = 0;
      local_110 = dVar15 / local_68;
      dVar13 = 0.0;
      local_108 = (dVar15 * dVar10 + dVar11) * dVar10;
      local_100 = (dVar12 + dVar11) * dVar14;
      pdVar2 = local_7c;
      do {
        dVar15 = (double)iVar5 + local_f0;
        dVar12 = dVar15 * local_e8;
        dVar16 = dVar15 + dVar15;
        dVar11 = cos(dVar12 * dVar10);
        dVar12 = cos(dVar12 * dVar14);
        dVar12 = ((dVar11 - dVar12) * local_110) / (dVar15 * dVar15) + *pdVar2;
        dVar11 = local_e0;
        if (dVar16 * dVar10 != dVar13) {
          dVar15 = dVar16 * dVar10 * local_b8;
          dVar11 = sin(dVar15);
          dVar11 = dVar11 / dVar15;
        }
        dVar11 = dVar11 * local_108;
        dVar15 = local_d8;
        if (dVar16 * dVar14 != dVar13) {
          dVar15 = dVar16 * dVar14 * local_b0;
          local_f8 = dVar11;
          dVar11 = sin(dVar15);
          dVar15 = dVar11 / dVar15;
          dVar11 = local_f8;
        }
        iVar5 = iVar5 + 1;
        *pdVar2 = (dVar11 - dVar15 * local_100) + dVar12;
        pdVar2 = pdVar2 + 1;
      } while (iVar6 != iVar5);
      pdVar1 = pdVar1 + 2;
      local_9c = local_9c + 2;
      if (local_80 == pdVar1) goto LAB_0001efe8;
    }
  }
LAB_0001efe8:
  if (local_a0 == 0) {
    if (0 < iVar6) {
      pdVar1 = (double *)((int)param_9 + local_70);
      pdVar3 = (double *)((int)pdVar1 + local_70);
      pdVar2 = param_9 + (int)(uVar8 - 1) / 2;
      do {
        dVar10 = *pdVar1;
        pdVar1 = pdVar1 + 1;
        *pdVar2 = dVar10;
        pdVar2 = pdVar2 + -1;
      } while (pdVar3 != pdVar1);
    }
LAB_0001f038:
    if (0 < (int)uVar8) goto LAB_0001f040;
LAB_0001f32c:
    if (param_3 < 1) {
      iVar6 = 1 << (local_74 & 0x1f);
      goto LAB_0001f228;
    }
LAB_0001f104:
    iVar6 = 0;
    pdVar1 = param_9;
    do {
      if (0 < param_4) {
        iVar5 = 0;
        dVar10 = 0.0;
        pdVar2 = pdVar1;
        do {
          dVar11 = *pdVar2;
          iVar5 = iVar5 + 1;
          pdVar2 = pdVar2 + param_3;
          dVar10 = dVar10 + dVar11;
        } while (param_4 != iVar5);
        iVar4 = 0;
        pdVar2 = pdVar1;
        do {
          iVar4 = iVar4 + 1;
          *pdVar2 = *pdVar2 / dVar10;
          pdVar2 = pdVar2 + param_3;
        } while (iVar5 != iVar4);
      }
      iVar6 = iVar6 + 1;
      pdVar1 = pdVar1 + 1;
    } while (iVar6 < param_3);
    iVar6 = 1 << (local_74 & 0x1f);
    if ((int)uVar8 < 1) goto LAB_0001f228;
  }
  else {
    pdVar2 = (double *)((int)param_9 + local_70);
    *pdVar2 = local_88;
    pdVar1 = pdVar2;
    if (iVar6 < 1) goto LAB_0001f038;
    do {
      pdVar2[-1] = pdVar1[1];
      pdVar2 = pdVar2 + -1;
      pdVar1 = pdVar1 + 1;
    } while (param_9 != pdVar2);
    if ((int)uVar8 < 1) goto LAB_0001f32c;
LAB_0001f040:
    pdVar1 = param_9;
    do {
      *pdVar1 = *pdVar1 + *pdVar1;
      pdVar1 = pdVar1 + 1;
    } while (pdVar1 != param_9 + uVar8);
    iVar6 = 0;
    dVar13 = *(double *)(local_98 + 0x25c0);
    dVar12 = 3.141592653589793;
    dVar11 = 0.0;
    dVar10 = (double)(int)(uVar8 - 1);
    pdVar1 = param_9;
    do {
      while (dVar14 = (double)iVar6 / dVar10 - dVar13, dVar14 != dVar11) {
        dVar14 = dVar14 * dVar12;
        iVar6 = iVar6 + 2;
        dVar15 = sin(dVar14);
        *pdVar1 = *pdVar1 * (dVar15 / dVar14);
        pdVar1 = pdVar1 + 1;
        if (uVar8 * 2 == iVar6) goto LAB_0001f0fc;
      }
      iVar6 = iVar6 + 2;
      *pdVar1 = *pdVar1 * dVar13;
      pdVar1 = pdVar1 + 1;
    } while (uVar8 * 2 != iVar6);
LAB_0001f0fc:
    if (0 < param_3) goto LAB_0001f104;
    iVar6 = 1 << (local_74 & 0x1f);
  }
  iVar5 = 0;
  dVar13 = *(double *)(local_98 + 0x25c0);
  dVar11 = *(double *)(local_94 + 0x2b70);
  dVar12 = (double)iVar6;
  dVar14 = -0.5;
  puVar7 = local_78;
  dVar10 = dVar13;
LAB_0001f1e0:
  do {
    dVar15 = modf(dVar12 * *param_9,(double *)&local_158);
    if (dVar15 <= dVar11) {
      if (dVar15 != dVar11) {
        if (dVar15 < dVar14) {
LAB_0001f298:
          uVar9 = (undefined2)(int)((double)CONCAT44(uStack_154,local_158) - dVar10);
        }
        else if (dVar15 == dVar14) {
          local_90 = local_158;
          uStack_8c = uStack_154 ^ 0x80000000;
          if (((int)-(double)CONCAT44(uStack_154,local_158) & 1U) != 0) goto LAB_0001f298;
          uVar9 = (undefined2)(int)(double)CONCAT44(uStack_154,local_158);
        }
        else {
          uVar9 = (undefined2)(int)(double)CONCAT44(uStack_154,local_158);
        }
LAB_0001f1c8:
        iVar5 = iVar5 + 1;
        *puVar7 = uVar9;
        puVar7 = puVar7 + 1;
        param_9 = param_9 + 1;
        if ((int)uVar8 <= iVar5) break;
        goto LAB_0001f1e0;
      }
      uVar9 = (undefined2)(int)(double)CONCAT44(uStack_154,local_158);
      if (((int)(double)CONCAT44(uStack_154,local_158) & 1U) == 0) goto LAB_0001f1c8;
    }
    iVar5 = iVar5 + 1;
    *puVar7 = (short)(int)((double)CONCAT44(uStack_154,local_158) + dVar13);
    puVar7 = puVar7 + 1;
    param_9 = param_9 + 1;
  } while (iVar5 < (int)uVar8);
LAB_0001f228:
  tunefilter(param_3,param_4,iVar6,local_78);
  return;
}



/* ===== FUNCTION designfilterff @ 0001f3c0 ===== */

void designfilterff(int param_1,int param_2,int param_3,int param_4,int param_5,uint param_6,
                   uint param_7,uint *param_8,short *param_9)

{
  bool bVar1;
  bool bVar2;
  longlong lVar3;
  longlong lVar4;
  bool bVar5;
  short sVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  uint *puVar10;
  uint *puVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint *puVar19;
  short *psVar20;
  uint uVar21;
  uint uVar22;
  int iVar23;
  int iVar24;
  uint uVar25;
  uint uVar26;
  int iVar27;
  uint uVar28;
  int iVar29;
  uint uVar30;
  int iVar31;
  int iVar32;
  int iVar33;
  int local_98;
  int local_94;
  int local_90;
  int local_8c [6];
  undefined1 *local_74;
  undefined1 *local_70;
  int local_6c;
  uint *local_68;
  undefined4 local_64;
  uint local_60;
  int *local_5c;
  int *local_58;
  uint *local_54;
  uint local_50;
  int local_4c;
  int local_48;
  uint local_44;
  uint local_40;
  int *local_3c;
  int local_38;
  short *local_34;
  int local_30;
  int local_2c;
  
  local_34 = param_9;
  local_44 = param_7;
  local_60 = param_3 * param_4 + param_5;
  local_54 = param_8;
  if (param_2 < param_1) {
    uVar25 = param_3 * 0x10000;
    uVar22 = param_3 * -0x10000;
    if (-1 < (int)uVar25) {
      uVar22 = uVar25;
    }
    uVar12 = 0x40000000;
    iVar15 = 1;
    do {
      iVar8 = iVar15 + 1;
      if ((uVar12 & uVar22) != 0) {
        uVar12 = 0x10 - iVar15;
        if ((int)uVar12 < 1) {
          iVar15 = uVar22 << (-uVar12 & 0x1f);
        }
        else {
          iVar15 = (int)uVar22 >> (uVar12 & 0x1f);
        }
        iVar8 = iVar15 * -2 + 0x2ed9e;
        lVar3 = (longlong)iVar15 * (longlong)iVar8;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)iVar8;
        uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        lVar3 = (longlong)iVar15 * (longlong)(int)uVar22;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)(int)uVar22;
        uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if ((int)uVar12 < 1) {
          uVar22 = uVar22 << (-uVar12 & 0x1f);
        }
        else {
          uVar22 = (int)uVar22 >> (uVar12 & 0x1f);
        }
        if ((int)uVar25 < 0) {
          uVar22 = -uVar22;
          iVar15 = (int)uVar22 >> 0x1f;
        }
        else {
          iVar15 = (int)uVar22 >> 0x1f;
        }
        goto LAB_0001f620;
      }
      uVar12 = uVar12 >> 1;
      iVar15 = iVar8;
    } while (iVar8 != 0x20);
    if ((int)uVar25 < 0) {
      uVar22 = 0x80000000;
      iVar15 = -1;
    }
    else {
      iVar15 = 0;
      uVar22 = 0x7fffffff;
    }
LAB_0001f620:
    uVar12 = param_1 * 0x10000;
    uVar25 = param_1 * -0x10000;
    if (-1 < (int)uVar12) {
      uVar25 = uVar12;
    }
    uVar26 = 0x40000000;
    iVar8 = 1;
    do {
      iVar23 = iVar8 + 1;
      if ((uVar26 & uVar25) != 0) {
        uVar26 = 0x10 - iVar8;
        if ((int)uVar26 < 1) {
          iVar8 = uVar25 << (-uVar26 & 0x1f);
        }
        else {
          iVar8 = (int)uVar25 >> (uVar26 & 0x1f);
        }
        iVar23 = iVar8 * -2 + 0x2ed9e;
        lVar3 = (longlong)iVar8 * (longlong)iVar23;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)iVar23;
        uVar25 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        lVar3 = (longlong)iVar8 * (longlong)(int)uVar25;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)(int)uVar25;
        uVar25 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if ((int)uVar26 < 1) {
          iVar8 = uVar25 << (-uVar26 & 0x1f);
        }
        else {
          iVar8 = (int)uVar25 >> (uVar26 & 0x1f);
        }
        if ((int)uVar12 < 0) {
          iVar8 = -iVar8;
        }
        goto LAB_0001f6fc;
      }
      uVar26 = uVar26 >> 1;
      iVar8 = iVar23;
    } while (iVar23 != 0x20);
    iVar8 = -0x80000000;
    if (-1 < (int)uVar12) {
      iVar8 = 0x7fffffff;
    }
LAB_0001f6fc:
    uVar25 = param_2 * iVar8;
    uVar22 = (iVar15 * uVar25 + ((int)uVar25 >> 0x1f) * uVar22 +
             (int)((ulonglong)uVar25 * (ulonglong)uVar22 >> 0x20)) * 0x10000 |
             (uint)((ulonglong)uVar25 * (ulonglong)uVar22) >> 0x10;
    iVar15 = (int)uVar22 >> 0x1f;
  }
  else {
    uVar25 = param_3 * 0x10000;
    uVar22 = param_3 * -0x10000;
    if (-1 < (int)uVar25) {
      uVar22 = uVar25;
    }
    uVar12 = 0x40000000;
    iVar15 = 1;
    do {
      iVar8 = iVar15 + 1;
      if ((uVar12 & uVar22) != 0) {
        uVar12 = 0x10 - iVar15;
        if ((int)uVar12 < 1) {
          iVar15 = uVar22 << (-uVar12 & 0x1f);
        }
        else {
          iVar15 = (int)uVar22 >> (uVar12 & 0x1f);
        }
        iVar8 = iVar15 * -2 + 0x2ed9e;
        lVar3 = (longlong)iVar15 * (longlong)iVar8;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)iVar8;
        uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        lVar3 = (longlong)iVar15 * (longlong)(int)uVar22;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)(int)uVar22;
        uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if ((int)uVar12 < 1) {
          uVar22 = uVar22 << (-uVar12 & 0x1f);
        }
        else {
          uVar22 = (int)uVar22 >> (uVar12 & 0x1f);
        }
        if ((int)uVar25 < 0) {
          uVar22 = -uVar22;
        }
        iVar15 = (int)uVar22 >> 0x1f;
        goto joined_r0x0001f51c;
      }
      uVar12 = uVar12 >> 1;
      iVar15 = iVar8;
    } while (iVar8 != 0x20);
    if ((int)uVar25 < 0) {
      uVar22 = 0x80000000;
      iVar15 = -1;
    }
    else {
      iVar15 = 0;
      uVar22 = 0x7fffffff;
    }
  }
joined_r0x0001f51c:
  local_50 = local_60 & 0x80000001;
  if ((int)local_50 < 0) {
    local_50 = ((local_60 & 0x80000001) - 1 | 0xfffffffe) + 1;
  }
  local_98 = 0;
  local_8c[3] = 0;
  local_8c[4] = 0;
  local_8c[1] = 0x10000;
  local_8c[2] = 0x10000;
  local_4c = (int)local_60 / 2;
  iVar15 = (int)((iVar15 * param_6 + ((int)param_6 >> 0x1f) * uVar22 +
                 (int)((ulonglong)param_6 * (ulonglong)uVar22 >> 0x20)) * 0x10000 |
                (uint)((ulonglong)param_6 * (ulonglong)uVar22) >> 0x10) >> 1;
  local_8c[0] = 0x8000;
  local_94 = iVar15;
  local_90 = iVar15;
  local_48 = param_3;
  if (0 < (int)local_60) {
    memset(param_8,0,local_60 * 4);
  }
  iVar8 = 0x10000;
  local_40 = 0;
  uVar22 = (uint)(local_50 != 0);
  local_2c = local_4c * 4;
  local_38 = local_2c + uVar22 * 4;
  local_6c = 0x8000;
  if (local_50 != 0) {
    local_6c = 0x10000;
  }
  local_58 = local_8c + 1;
  local_3c = local_8c + 5;
  local_68 = local_54 + uVar22 + local_4c * 2;
  local_64 = 0x7fffffff;
  local_5c = &local_98;
  do {
    iVar33 = *local_5c;
    iVar23 = 1;
    uVar12 = 0x40000000;
    uVar26 = iVar15 - iVar33;
    uVar28 = local_58[1] - iVar8;
    uVar25 = -uVar26;
    if (-1 < (int)uVar26) {
      uVar25 = uVar26;
    }
    do {
      iVar24 = iVar23 + 1;
      if ((uVar12 & uVar25) != 0) {
        uVar12 = 0x10 - iVar23;
        if ((int)uVar12 < 1) {
          iVar23 = uVar25 << (-uVar12 & 0x1f);
        }
        else {
          iVar23 = (int)uVar25 >> (uVar12 & 0x1f);
        }
        iVar24 = iVar23 * -2 + 0x2ed9e;
        lVar3 = (longlong)iVar23 * (longlong)iVar24;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)iVar24;
        uVar25 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        lVar3 = (longlong)iVar23 * (longlong)(int)uVar25;
        lVar3 = (longlong)
                (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                (longlong)(int)uVar25;
        uVar25 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if ((int)uVar12 < 1) {
          uVar25 = uVar25 << (-uVar12 & 0x1f);
        }
        else {
          uVar25 = (int)uVar25 >> (uVar12 & 0x1f);
        }
        if ((int)uVar26 < 0) {
          uVar25 = -uVar25;
        }
        iVar23 = (int)uVar25 >> 0x1f;
        goto LAB_0001f93c;
      }
      uVar12 = uVar12 >> 1;
      iVar23 = iVar24;
    } while (iVar24 != 0x20);
    uVar25 = 0x7fffffff;
    if ((int)uVar26 < 0) {
      uVar25 = 0x80000000;
    }
    iVar23 = -1;
    if ((int)uVar26 >= 0) {
      iVar23 = 0;
    }
LAB_0001f93c:
    uVar25 = (iVar23 * uVar28 + ((int)uVar28 >> 0x1f) * uVar25 +
             (int)((ulonglong)uVar28 * (ulonglong)uVar25 >> 0x20)) * 0x10000 |
             (uint)((ulonglong)uVar28 * (ulonglong)uVar25) >> 0x10;
    iVar23 = iVar8 - ((int)((ulonglong)((longlong)(int)uVar25 * (longlong)iVar33) >> 0x20) << 0x10 |
                     (uint)((longlong)(int)uVar25 * (longlong)iVar33) >> 0x10);
    if (local_50 != 0) {
      lVar3 = (longlong)iVar23 * (longlong)(int)uVar26;
      lVar4 = (longlong)
              (int)(((int)((ulonglong)((longlong)iVar15 * (longlong)iVar15) >> 0x20) << 0x10 |
                    (uint)((longlong)iVar15 * (longlong)iVar15) >> 0x10) -
                   ((int)((ulonglong)((longlong)iVar33 * (longlong)iVar33) >> 0x20) << 0x10 |
                   (uint)((longlong)iVar33 * (longlong)iVar33) >> 0x10)) *
              (longlong)((int)uVar25 >> 1);
      local_40 = local_40 +
                 ((int)((ulonglong)lVar4 >> 0x20) << 0x10 | (uint)lVar4 >> 0x10) +
                 ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10);
    }
    if (0 < local_4c) {
      puVar10 = local_54 + uVar22 + local_4c;
      iVar24 = 0;
      lVar3 = (longlong)
              (int)(iVar23 + ((int)((ulonglong)((longlong)(int)uVar25 * (longlong)iVar15) >> 0x20)
                              << 0x10 | (uint)((longlong)(int)uVar25 * (longlong)iVar15) >> 0x10)) *
              (longlong)iVar15;
      uVar12 = (int)((ulonglong)((longlong)iVar33 * (longlong)iVar8) >> 0x20) << 0x10 |
               (uint)((longlong)iVar33 * (longlong)iVar8) >> 0x10;
      lVar4 = (longlong)(int)uVar25 * 0x67c >> 0x10;
      uVar25 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
      local_8c[5] = (int)uVar25 >> 0x1f;
      local_74 = (undefined1 *)((int)uVar12 >> 0x1f);
      local_70 = (undefined1 *)((ulonglong)lVar4 >> 0x20);
      do {
        iVar8 = iVar24 + local_6c;
        uVar28 = (int)((ulonglong)((longlong)iVar8 * (longlong)iVar15) >> 0x20) << 0x10 |
                 (uint)((longlong)iVar8 * (longlong)iVar15) >> 0x10;
        uVar26 = uVar28 * 2;
        if ((int)uVar26 < 0) {
          uVar26 = uVar28 * -2;
        }
        uVar26 = uVar26 & 0x1ffff;
        if (0xffff < uVar26) {
          uVar26 = 0x20000 - uVar26;
        }
        bVar1 = 0x8000 < (int)uVar26;
        if (bVar1) {
          uVar26 = 0x10000 - uVar26;
        }
        iVar23 = (int)uVar26 >> 7;
        if (iVar23 == 0) {
          uVar28 = 0x10000;
LAB_0002016c:
          uVar16 = (uint)*(ushort *)(cos_lut + iVar23 * 2);
        }
        else {
          uVar28 = (uint)*(ushort *)(cos_lut + (iVar23 + -1) * 2);
          if (iVar23 != 0x100) goto LAB_0002016c;
          uVar16 = 0xfffffe6e;
        }
        uVar13 = (int)((ulonglong)((longlong)iVar33 * (longlong)iVar8) >> 0x20) << 0x10 |
                 (uint)((longlong)iVar33 * (longlong)iVar8) >> 0x10;
        uVar14 = uVar13 * 2;
        if ((int)uVar14 < 0) {
          uVar14 = uVar13 * -2;
        }
        iVar23 = (int)((uVar26 & 0x7f) * uVar16 + (0x80 - (uVar26 & 0x7f)) * uVar28 + 0x40) >> 7;
        uVar14 = uVar14 & 0x1ffff;
        if (bVar1) {
          iVar23 = -iVar23;
        }
        if (0xffff < uVar14) {
          uVar14 = 0x20000 - uVar14;
        }
        bVar1 = 0x8000 < (int)uVar14;
        if (bVar1) {
          uVar14 = 0x10000 - uVar14;
        }
        iVar9 = (int)uVar14 >> 7;
        if (iVar9 == 0) {
          uVar26 = 0x10000;
LAB_0002017c:
          uVar28 = (uint)*(ushort *)(cos_lut + iVar9 * 2);
        }
        else {
          uVar26 = (uint)*(ushort *)(cos_lut + (iVar9 + -1) * 2);
          if (iVar9 != 0x100) goto LAB_0002017c;
          uVar28 = 0xfffffe6e;
        }
        lVar3 = (longlong)(iVar8 * 2) * (longlong)iVar15;
        iVar9 = (int)((uVar14 & 0x7f) * uVar28 + (0x80 - (uVar14 & 0x7f)) * uVar26 + 0x40) >> 7;
        if (bVar1) {
          iVar9 = -iVar9;
        }
        uVar26 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if (uVar26 == 0) {
          uVar26 = 0x10000;
          iVar27 = 0;
        }
        else {
          uVar28 = 0x8000 - uVar26;
          if ((int)uVar28 < 0) {
            uVar28 = -uVar28;
          }
          uVar28 = uVar28 & 0x1ffff;
          if (0xffff < uVar28) {
            uVar28 = 0x20000 - uVar28;
          }
          bVar1 = 0x8000 < (int)uVar28;
          if (bVar1) {
            uVar28 = 0x10000 - uVar28;
          }
          iVar27 = (int)uVar28 >> 7;
          if (iVar27 == 0) {
            uVar16 = 0x10000;
LAB_000201e4:
            uVar14 = (uint)*(ushort *)(cos_lut + iVar27 * 2);
          }
          else {
            uVar16 = (uint)*(ushort *)(cos_lut + (iVar27 + -1) * 2);
            if (iVar27 != 0x100) goto LAB_000201e4;
            uVar14 = 0xfffffe6e;
          }
          uVar13 = -uVar26;
          if (-1 < (int)uVar26) {
            uVar13 = uVar26;
          }
          iVar27 = 1;
          uVar17 = 0x40000000;
          iVar29 = (int)((uVar28 & 0x7f) * uVar14 + (0x80 - (uVar28 & 0x7f)) * uVar16 + 0x40) >> 7;
          if (bVar1) {
            iVar29 = -iVar29;
          }
          do {
            iVar31 = iVar27 + 1;
            if ((uVar17 & uVar13) != 0) {
              uVar28 = 0x10 - iVar27;
              if ((int)uVar28 < 1) {
                iVar27 = uVar13 << (-uVar28 & 0x1f);
              }
              else {
                iVar27 = (int)uVar13 >> (uVar28 & 0x1f);
              }
              iVar31 = iVar27 * -2 + 0x2ed9e;
              lVar3 = (longlong)iVar27 * (longlong)iVar31;
              lVar3 = (longlong)
                      (int)(0x20000 -
                           ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                      (longlong)iVar31;
              uVar16 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
              lVar3 = (longlong)iVar27 * (longlong)(int)uVar16;
              lVar3 = (longlong)
                      (int)(0x20000 -
                           ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                      (longlong)(int)uVar16;
              uVar16 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
              if ((int)uVar28 < 1) {
                iVar27 = uVar16 << (-uVar28 & 0x1f);
              }
              else {
                iVar27 = (int)uVar16 >> (uVar28 & 0x1f);
              }
              if ((int)uVar26 < 0) {
                iVar27 = -iVar27;
              }
              lVar3 = (longlong)iVar27 * 0x517c >> 0x10;
              goto LAB_0001fd6c;
            }
            uVar17 = uVar17 >> 1;
            iVar27 = iVar31;
          } while (iVar31 != 0x20);
          if ((int)uVar26 < 0) {
            lVar3 = -0x28be0000;
          }
          else {
            lVar3 = 0x28bdffff;
          }
LAB_0001fd6c:
          iVar31 = (int)((ulonglong)(lVar3 * iVar29) >> 0x20);
          iVar27 = iVar31 >> 0x10;
          uVar26 = iVar31 * 0x10000 | (uint)(lVar3 * iVar29) >> 0x10;
        }
        lVar3 = (longlong)iVar33 * (longlong)(iVar8 * 2);
        uVar28 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
        if (uVar28 == 0) {
          uVar28 = 0x10000;
          iVar29 = 0;
        }
        else {
          uVar16 = 0x8000 - uVar28;
          if ((int)uVar16 < 0) {
            uVar16 = -uVar16;
          }
          uVar16 = uVar16 & 0x1ffff;
          if (0xffff < uVar16) {
            uVar16 = 0x20000 - uVar16;
          }
          bVar1 = 0x8000 < (int)uVar16;
          if (bVar1) {
            uVar16 = 0x10000 - uVar16;
          }
          iVar29 = (int)uVar16 >> 7;
          if (iVar29 == 0) {
            uVar14 = 0x10000;
LAB_000201d0:
            uVar13 = (uint)*(ushort *)(cos_lut + iVar29 * 2);
          }
          else {
            uVar14 = (uint)*(ushort *)(cos_lut + (iVar29 + -1) * 2);
            if (iVar29 != 0x100) goto LAB_000201d0;
            uVar13 = 0xfffffe6e;
          }
          uVar17 = -uVar28;
          if (-1 < (int)uVar28) {
            uVar17 = uVar28;
          }
          iVar29 = 1;
          uVar18 = 0x40000000;
          iVar31 = (int)((uVar16 & 0x7f) * uVar13 + (0x80 - (uVar16 & 0x7f)) * uVar14 + 0x40) >> 7;
          if (bVar1) {
            iVar31 = -iVar31;
          }
          do {
            iVar32 = iVar29 + 1;
            if ((uVar18 & uVar17) != 0) {
              uVar16 = 0x10 - iVar29;
              if ((int)uVar16 < 1) {
                iVar29 = uVar17 << (-uVar16 & 0x1f);
              }
              else {
                iVar29 = (int)uVar17 >> (uVar16 & 0x1f);
              }
              iVar32 = iVar29 * -2 + 0x2ed9e;
              lVar3 = (longlong)iVar29 * (longlong)iVar32;
              lVar3 = (longlong)
                      (int)(0x20000 -
                           ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                      (longlong)iVar32;
              uVar14 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
              lVar3 = (longlong)iVar29 * (longlong)(int)uVar14;
              lVar3 = (longlong)
                      (int)(0x20000 -
                           ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                      (longlong)(int)uVar14;
              uVar14 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
              if ((int)uVar16 < 1) {
                iVar29 = uVar14 << (-uVar16 & 0x1f);
              }
              else {
                iVar29 = (int)uVar14 >> (uVar16 & 0x1f);
              }
              if ((int)uVar28 < 0) {
                iVar29 = -iVar29;
              }
              lVar3 = (longlong)iVar29 * 0x517c >> 0x10;
              goto LAB_0001ff54;
            }
            uVar18 = uVar18 >> 1;
            iVar29 = iVar32;
          } while (iVar32 != 0x20);
          if ((int)uVar28 < 0) {
            lVar3 = -0x28be0000;
          }
          else {
            lVar3 = 0x28bdffff;
          }
LAB_0001ff54:
          iVar32 = (int)((ulonglong)(lVar3 * iVar31) >> 0x20);
          iVar29 = iVar32 >> 0x10;
          uVar28 = iVar32 * 0x10000 | (uint)(lVar3 * iVar31) >> 0x10;
        }
        uVar14 = (int)((ulonglong)((longlong)iVar8 * (longlong)iVar8) >> 0x20) << 0x10 |
                 (uint)((longlong)iVar8 * (longlong)iVar8) >> 0x10;
        lVar3 = lVar4 * (iVar23 - iVar9);
        iVar8 = (int)((ulonglong)lVar3 >> 0x20);
        uVar13 = iVar8 * 0x10000 | (uint)lVar3 >> 0x10;
        uVar16 = -uVar14;
        if (-1 < (int)uVar14) {
          uVar16 = uVar14;
        }
        uVar17 = 0x40000000;
        iVar23 = 1;
        do {
          iVar9 = iVar23 + 1;
          if ((uVar17 & uVar16) != 0) {
            uVar17 = 0x10 - iVar23;
            if ((int)uVar17 < 1) {
              iVar23 = uVar16 << (-uVar17 & 0x1f);
            }
            else {
              iVar23 = (int)uVar16 >> (uVar17 & 0x1f);
            }
            iVar9 = iVar23 * -2 + 0x2ed9e;
            lVar3 = (longlong)iVar23 * (longlong)iVar9;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)iVar9;
            uVar16 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            lVar3 = (longlong)iVar23 * (longlong)(int)uVar16;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)(int)uVar16;
            uVar16 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            if ((int)uVar17 < 1) {
              uVar16 = uVar16 << (-uVar17 & 0x1f);
            }
            else {
              uVar16 = (int)uVar16 >> (uVar17 & 0x1f);
            }
            if ((int)uVar14 < 0) {
              uVar16 = -uVar16;
            }
            iVar23 = (int)uVar16 >> 0x1f;
            goto LAB_000200d4;
          }
          uVar17 = uVar17 >> 1;
          iVar23 = iVar9;
        } while (iVar9 != 0x20);
        uVar16 = 0x7fffffff;
        if ((int)uVar14 < 0) {
          uVar16 = 0x80000000;
        }
        iVar23 = -1;
        if ((int)uVar14 >= 0) {
          iVar23 = 0;
        }
LAB_000200d4:
        lVar3 = (ulonglong)uVar13 * (ulonglong)uVar16;
        *puVar10 = *puVar10 +
                   ((iVar23 * uVar13 + (iVar8 >> 0x10) * uVar16 + (int)((ulonglong)lVar3 >> 0x20)) *
                    0x10000 | (uint)lVar3 >> 0x10) +
                   (((iVar27 * uVar25 + local_8c[5] * uVar26 +
                     (int)((ulonglong)uVar25 * (ulonglong)uVar26 >> 0x20)) * 0x10000 |
                    (uint)((ulonglong)uVar25 * (ulonglong)uVar26) >> 0x10) -
                   ((iVar29 * uVar12 + (int)local_74 * uVar28 +
                    (int)((ulonglong)uVar12 * (ulonglong)uVar28 >> 0x20)) * 0x10000 |
                   (uint)((ulonglong)uVar12 * (ulonglong)uVar28) >> 0x10));
        puVar10 = puVar10 + 1;
        iVar24 = iVar24 + 0x10000;
      } while (local_68 != puVar10);
    }
    local_58 = local_58 + 2;
    if (local_3c == local_58) break;
    iVar8 = *local_58;
    iVar15 = local_5c[3];
    local_5c = local_5c + 2;
  } while( true );
  if (local_50 == 0) {
    if (0 < local_4c) {
      puVar10 = local_54 + local_4c;
      puVar19 = puVar10 + local_4c;
      puVar11 = local_54 + (int)(local_60 - 1) / 2;
      do {
        uVar22 = *puVar10;
        puVar10 = puVar10 + 1;
        *puVar11 = uVar22;
        puVar11 = puVar11 + -1;
      } while (puVar19 != puVar10);
    }
LAB_000202bc:
    if (0 < (int)local_60) goto LAB_000202c8;
LAB_00020a10:
    if (local_48 < 1) {
      iVar15 = 1 << (local_44 & 0x1f);
      goto LAB_000206d8;
    }
LAB_000204dc:
    iVar15 = 0;
    puVar10 = local_54;
    do {
      if (param_4 < 1) {
        uVar25 = 0;
        uVar22 = 0;
      }
      else {
        iVar8 = 0;
        uVar25 = 0;
        puVar11 = puVar10;
        do {
          uVar22 = *puVar11;
          iVar8 = iVar8 + 1;
          puVar11 = puVar11 + local_48;
          uVar25 = uVar25 + uVar22;
        } while (param_4 != iVar8);
        uVar22 = -uVar25;
        if (-1 < (int)uVar25) {
          uVar22 = uVar25;
        }
      }
      uVar12 = 0x40000000;
      iVar8 = 1;
      do {
        iVar23 = iVar8 + 1;
        if ((uVar12 & uVar22) != 0) {
          uVar12 = 0x10 - iVar8;
          if ((int)uVar12 < 1) {
            iVar8 = uVar22 << (-uVar12 & 0x1f);
          }
          else {
            iVar8 = (int)uVar22 >> (uVar12 & 0x1f);
          }
          iVar23 = iVar8 * -2 + 0x2ed9e;
          lVar3 = (longlong)iVar23 * (longlong)iVar8;
          lVar3 = (longlong)
                  (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                  (longlong)iVar23;
          uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
          lVar3 = (longlong)(int)uVar22 * (longlong)iVar8;
          lVar3 = (longlong)
                  (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10)) *
                  (longlong)(int)uVar22;
          uVar22 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
          if ((int)uVar12 < 1) {
            iVar8 = uVar22 << (-uVar12 & 0x1f);
          }
          else {
            iVar8 = (int)uVar22 >> (uVar12 & 0x1f);
          }
          if ((int)uVar25 < 0) {
            iVar8 = -iVar8;
          }
          goto LAB_000205f0;
        }
        uVar12 = uVar12 >> 1;
        iVar8 = iVar23;
      } while (iVar23 != 0x20);
      iVar8 = -0x80000000;
      if (-1 < (int)uVar25) {
        iVar8 = 0x7fffffff;
      }
LAB_000205f0:
      iVar23 = 0;
      puVar11 = puVar10;
      if (0 < param_4) {
        do {
          iVar23 = iVar23 + 1;
          *puVar11 = (int)((ulonglong)((longlong)iVar8 * (longlong)(int)*puVar11) >> 0x20) << 0x10 |
                     (uint)((longlong)iVar8 * (longlong)(int)*puVar11) >> 0x10;
          puVar11 = puVar11 + local_48;
        } while (param_4 != iVar23);
      }
      iVar15 = iVar15 + 1;
      puVar10 = puVar10 + 1;
    } while (iVar15 < local_48);
    iVar15 = 1 << (local_44 & 0x1f);
    if ((int)local_60 < 1) goto LAB_000206d8;
  }
  else {
    puVar11 = local_54 + local_4c;
    *puVar11 = local_40;
    puVar10 = puVar11;
    if (local_4c < 1) goto LAB_000202bc;
    do {
      puVar11[-1] = puVar10[1];
      puVar11 = puVar11 + -1;
      puVar10 = puVar10 + 1;
    } while (local_54 != puVar11);
    if ((int)local_60 < 1) goto LAB_00020a10;
LAB_000202c8:
    puVar10 = local_54;
    do {
      *puVar10 = *puVar10 << 1;
      puVar10 = puVar10 + 1;
    } while (local_54 + local_60 != puVar10);
    local_70 = &DAT_00030000;
    uVar25 = 0;
    local_74 = &DAT_00030000;
    bVar5 = false;
    uVar12 = (local_60 - 1) * 0x10000;
    bVar1 = false;
    uVar22 = (local_60 - 1) * -0x10000;
    if (-1 < (int)uVar12) {
      uVar22 = uVar12;
    }
    local_8c[5] = 0x7fffffff;
    uVar26 = den_2714;
    uVar28 = last_n_2713;
    puVar11 = local_54;
    uVar16 = last_n_2713;
    do {
      uVar14 = *puVar11;
      if (local_60 != uVar28) {
        uVar26 = 0x40000000;
        iVar15 = 1;
        do {
          iVar8 = iVar15 + 1;
          if ((uVar26 & uVar22) != 0) {
            uVar28 = 0x10 - iVar15;
            if ((int)uVar28 < 1) {
              iVar15 = uVar22 << (-uVar28 & 0x1f);
            }
            else {
              iVar15 = (int)uVar22 >> (uVar28 & 0x1f);
            }
            iVar8 = iVar15 * -2 + 0x2ed9e;
            lVar3 = (longlong)iVar15 * (longlong)iVar8;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)iVar8;
            uVar26 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            lVar3 = (longlong)iVar15 * (longlong)(int)uVar26;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)(int)uVar26;
            uVar26 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            if ((int)uVar28 < 1) {
              uVar26 = uVar26 << (-uVar28 & 0x1f);
            }
            else {
              uVar26 = (int)uVar26 >> (uVar28 & 0x1f);
            }
            if ((int)uVar12 < 0) {
              uVar26 = -uVar26;
            }
            goto LAB_00020434;
          }
          uVar26 = uVar26 >> 1;
          iVar15 = iVar8;
        } while (iVar8 != 0x20);
        uVar26 = 0x7fffffff;
        if ((int)uVar12 < 0) {
          uVar26 = 0x80000000;
        }
LAB_00020434:
        bVar5 = true;
        bVar1 = true;
        uVar16 = local_60;
      }
      uVar28 = (((int)uVar25 >> 0x1f) * uVar26 +
               (int)((ulonglong)uVar25 * (ulonglong)uVar26 >> 0x20)) * 0x10000 |
               (uint)((ulonglong)uVar25 * (ulonglong)uVar26) >> 0x10;
      uVar17 = uVar28 - 0x10000;
      uVar13 = 0x10000;
      if (uVar17 == 0) {
        iVar15 = 0;
      }
      else {
        uVar13 = 0x8000 - uVar17;
        if ((int)uVar13 < 0) {
          uVar13 = -uVar13;
        }
        uVar13 = uVar13 & 0x1ffff;
        if (0xffff < uVar13) {
          uVar13 = 0x20000 - uVar13;
        }
        bVar2 = 0x8000 < (int)uVar13;
        if (bVar2) {
          uVar13 = 0x10000 - uVar13;
        }
        iVar15 = (int)uVar13 >> 7;
        if (iVar15 == 0) {
          uVar18 = 0x10000;
LAB_00020904:
          uVar30 = (uint)*(ushort *)(cos_lut + iVar15 * 2);
        }
        else {
          uVar18 = (uint)*(ushort *)(cos_lut + (iVar15 + -1) * 2);
          if (iVar15 != 0x100) goto LAB_00020904;
          uVar30 = 0xfffffe6e;
        }
        uVar21 = uVar17;
        if ((int)uVar17 < 0) {
          uVar21 = 0x10000 - uVar28;
        }
        iVar15 = 1;
        uVar28 = 0x40000000;
        iVar8 = (int)((uVar13 & 0x7f) * uVar30 + (0x80 - (uVar13 & 0x7f)) * uVar18 + 0x40) >> 7;
        if (bVar2) {
          iVar8 = -iVar8;
        }
        do {
          iVar23 = iVar15 + 1;
          if ((uVar28 & uVar21) != 0) {
            uVar28 = 0x10 - iVar15;
            if ((int)uVar28 < 1) {
              iVar15 = uVar21 << (-uVar28 & 0x1f);
            }
            else {
              iVar15 = (int)uVar21 >> (uVar28 & 0x1f);
            }
            iVar23 = iVar15 * -2 + 0x2ed9e;
            lVar3 = (longlong)iVar15 * (longlong)iVar23;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)iVar23;
            uVar13 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            lVar3 = (longlong)iVar15 * (longlong)(int)uVar13;
            lVar3 = (longlong)
                    (int)(0x20000 - ((int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10))
                    * (longlong)(int)uVar13;
            uVar13 = (int)((ulonglong)lVar3 >> 0x20) << 0x10 | (uint)lVar3 >> 0x10;
            if ((int)uVar28 < 1) {
              iVar15 = uVar13 << (-uVar28 & 0x1f);
            }
            else {
              iVar15 = (int)uVar13 >> (uVar28 & 0x1f);
            }
            if ((int)uVar17 < 0) {
              iVar15 = -iVar15;
            }
            lVar3 = (longlong)iVar15 * 0x517c >> 0x10;
            goto LAB_00020890;
          }
          uVar28 = uVar28 >> 1;
          iVar15 = iVar23;
        } while (iVar23 != 0x20);
        if ((int)uVar17 < 0) {
          lVar3 = -0x28be0000;
        }
        else {
          lVar3 = 0x28bdffff;
        }
LAB_00020890:
        iVar23 = (int)((ulonglong)(lVar3 * iVar8) >> 0x20);
        iVar15 = iVar23 >> 0x10;
        uVar13 = iVar23 * 0x10000 | (uint)(lVar3 * iVar8) >> 0x10;
      }
      uVar25 = uVar25 + 0x20000;
      *puVar11 = (iVar15 * uVar14 + ((int)uVar14 >> 0x1f) * uVar13 +
                 (int)((ulonglong)uVar14 * (ulonglong)uVar13 >> 0x20)) * 0x10000 |
                 (uint)((ulonglong)uVar14 * (ulonglong)uVar13) >> 0x10;
      puVar11 = puVar11 + 1;
      uVar28 = local_60;
    } while (puVar11 != puVar10);
    if (bVar1) {
      den_2714 = uVar26;
    }
    if (bVar5) {
      last_n_2713 = uVar16;
    }
    if (0 < local_48) goto LAB_000204dc;
    iVar15 = 1 << (local_44 & 0x1f);
  }
  iVar8 = 0;
  psVar20 = local_34;
  puVar10 = local_54;
  do {
    while( true ) {
      iVar8 = iVar8 + 1;
      uVar25 = *puVar10 << (local_44 & 0x1f);
      uVar22 = -uVar25;
      if (-1 < (int)uVar25) {
        uVar22 = uVar25;
      }
      sVar6 = (short)(uVar22 >> 0x10);
      if (((uVar22 & 0xffff) < 0x8001) &&
         (((uVar22 & 0xffff) != 0x8000 || (((int)uVar22 >> 0x10 & 1U) == 0)))) break;
      sVar7 = sVar6 + 1;
      if ((int)uVar25 < 0) {
        sVar7 = -(sVar6 + 1);
      }
      *psVar20 = sVar7;
      psVar20 = psVar20 + 1;
      puVar10 = puVar10 + 1;
      if (iVar8 >= (int)local_60) goto LAB_000206d8;
    }
    if ((int)uVar25 < 0) {
      sVar6 = -sVar6;
    }
    *psVar20 = sVar6;
    psVar20 = psVar20 + 1;
    puVar10 = puVar10 + 1;
  } while (iVar8 < (int)local_60);
LAB_000206d8:
  local_5c = local_5c + 2;
  local_30 = param_4;
  tunefilter(local_48,param_4,iVar15,local_34);
  return;
}



/* ===== FUNCTION extract_phase @ 00020b38 ===== */

void extract_phase(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined2 uVar1;
  undefined2 *puVar2;
  undefined2 *puVar3;
  undefined2 *puVar4;
  
  puVar4 = (undefined2 *)(param_2 + param_5 * 2);
  if (0 < param_4) {
    puVar2 = (undefined2 *)(param_1 + param_4 * 2 + -2);
    do {
      uVar1 = *puVar4;
      puVar3 = puVar2 + -1;
      puVar4 = puVar4 + param_3;
      *puVar2 = uVar1;
      puVar2 = puVar3;
    } while ((undefined2 *)(param_1 + -2) != puVar3);
  }
  return;
}



/* ===== FUNCTION extract_coef @ 00020b78 ===== */

int extract_coef(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  return (int)*(short *)(param_1 + (((param_3 + -1) - param_5) * param_2 + param_4) * 2);
}



/* ===== FUNCTION __do_global_ctors_aux @ 00020ba0 ===== */

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



/* ===== FUNCTION pthread_mutex_unlock @ 00020c00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_unlock(pthread_mutex_t *__mutex)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION cos @ 00020c10 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double cos(double __x)

{
  double in_f0_1;
  
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
  (*(code *)0x2b00)();
  return in_f0_1;
}



/* ===== FUNCTION strcmp @ 00020c20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION read @ 00020c30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t read(int __fd,void *__buf,size_t __nbytes)

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
  sVar1 = (*(code *)0x2b00)();
  return sVar1;
}



/* ===== FUNCTION pthread_mutex_destroy @ 00020c40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_destroy(pthread_mutex_t *__mutex)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION printf @ 00020c50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION free @ 00020c60 ===== */

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION memcpy @ 00020c70 ===== */

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
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION shmget @ 00020c80 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmget(key_t __key,size_t __size,int __shmflg)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_mutex_init @ 00020c90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_init(pthread_mutex_t *__mutex,pthread_mutexattr_t *__mutexattr)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION mmap64 @ 00020ca0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap64(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off64_t __offset)

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
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION pthread_cond_wait @ 00020cb0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_cond_wait(pthread_cond_t *__cond,pthread_mutex_t *__mutex)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION sincos @ 00020cc0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void sincos(double __x,double *__sinx,double *__cosx)

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION msgctl @ 00020cd0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgctl(int __msqid,int __cmd,msqid_ds *__buf)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION shmat @ 00020ce0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * shmat(int __shmid,void *__shmaddr,int __shmflg)

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
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION pthread_mutex_lock @ 00020cf0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_lock(pthread_mutex_t *__mutex)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION ioctl @ 00020d00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ioctl(int __fd,ulong __request,...)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION perror @ 00020d10 ===== */

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION msgrcv @ 00020d20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t msgrcv(int __msqid,void *__msgp,size_t __msgsz,long __msgtyp,int __msgflg)

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
  sVar1 = (*(code *)0x2b00)();
  return sVar1;
}



/* ===== FUNCTION usleep @ 00020d30 ===== */

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_create @ 00020d40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_create(pthread_t *__newthread,pthread_attr_t *__attr,__start_routine *__start_routine,
                  void *__arg)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_cond_signal @ 00020d50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_cond_signal(pthread_cond_t *__cond)

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
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION getenv @ 00020d60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * getenv(char *__name)

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
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pcVar1 = (char *)(*(code *)0x2b00)();
  return pcVar1;
}



/* ===== FUNCTION malloc @ 00020d70 ===== */

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
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION puts @ 00020d80 ===== */

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION system @ 00020d90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

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
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION __memcpy_chk @ 00020da0 ===== */

void __memcpy_chk(void)

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION modf @ 00020db0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double modf(double __x,double *__iptr)

{
  int unaff_gp;
  double in_f0_1;
  
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
  (*(code *)0x2b00)();
  return in_f0_1;
}



/* ===== FUNCTION exit @ 00020dc0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void exit(int __status)

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION strlen @ 00020dd0 ===== */

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
  sVar1 = (*(code *)0x2b00)();
  return sVar1;
}



/* ===== FUNCTION fsync @ 00020de0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fsync(int __fd)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION sin @ 00020df0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double sin(double __x)

{
  int unaff_gp;
  double in_f0_1;
  
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
  (*(code *)0x2b00)();
  return in_f0_1;
}



/* ===== FUNCTION pthread_attr_init @ 00020e00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_init(pthread_attr_t *__attr)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION fprintf @ 00020e10 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fprintf(FILE *__stream,char *__format,...)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION write @ 00020e20 ===== */

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
  sVar1 = (*(code *)0x2b00)();
  return sVar1;
}



/* ===== FUNCTION pthread_cond_init @ 00020e30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_cond_init(pthread_cond_t *__cond,pthread_condattr_t *__cond_attr)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION open64 @ 00020e40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int open64(char *__file,int __oflag,...)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION memset @ 00020e50 ===== */

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
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION snprintf @ 00020e60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int snprintf(char *__s,size_t __maxlen,char *__format,...)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION strncpy @ 00020e70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strncpy(char *__dest,char *__src,size_t __n)

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
  pcVar1 = (char *)(*(code *)0x2b00)();
  return pcVar1;
}



/* ===== FUNCTION cacheflush @ 00020e80 ===== */

void cacheflush(void)

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION access @ 00020e90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int access(char *__name,int __type)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_exit @ 00020ea0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void pthread_exit(void *__retval)

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
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION pthread_attr_setstacksize @ 00020eb0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_setstacksize(pthread_attr_t *__attr,size_t __stacksize)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION munmap @ 00020ec0 ===== */

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
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_cond_destroy @ 00020ed0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_cond_destroy(pthread_cond_t *__cond)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION fclose @ 00020ee0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fclose(FILE *__stream)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION pthread_join @ 00020ef0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_join(pthread_t __th,void **__thread_return)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION ftok @ 00020f00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

key_t ftok(char *__pathname,int __proj_id)

{
  key_t kVar1;
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
  kVar1 = (*(code *)0x2b00)();
  return kVar1;
}



/* ===== FUNCTION fopen64 @ 00020f10 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fopen64(char *__filename,char *__modes)

{
  FILE *pFVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pFVar1 = (FILE *)(*(code *)0x2b00)();
  return pFVar1;
}



/* ===== FUNCTION msgget @ 00020f20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgget(key_t __key,int __msgflg)

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
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION strncmp @ 00020f30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncmp(char *__s1,char *__s2,size_t __n)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION msgsnd @ 00020f40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgsnd(int __msqid,void *__msgp,size_t __msgsz,int __msgflg)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION __xstat64 @ 00020f50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __xstat64(int __ver,char *__filename,stat64 *__stat_buf)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION strtol @ 00020f60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long strtol(char *__nptr,char **__endptr,int __base)

{
  long lVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  lVar1 = (*(code *)0x2b00)();
  return lVar1;
}



/* ===== FUNCTION close @ 00020f70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x2b00)();
  return iVar1;
}



/* ===== FUNCTION calloc @ 00020f80 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * calloc(size_t __nmemb,size_t __size)

{
  void *pvVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  pvVar1 = (void *)(*(code *)0x2b00)();
  return pvVar1;
}



/* ===== FUNCTION _fini @ 00020fa0 ===== */

void _fini(void)

{
  (*(code *)0x2b00)();
  return;
}



/* ===== FUNCTION _ITM_registerTMCloneTable @ 00034000 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _ITM_registerTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _Jv_RegisterClasses @ 00034004 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _Jv_RegisterClasses(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __gmon_start__ @ 00034008 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __gmon_start__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _ITM_deregisterTMCloneTable @ 00034010 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _ITM_deregisterTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __cxa_finalize @ 00034014 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __cxa_finalize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_finalize@@GLIBC_2.2 */
  halt_baddata();
}


