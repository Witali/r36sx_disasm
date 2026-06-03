
/* ===== FORCED FUNCTION forced_0042a9a0 @ 0042a9a0 ===== */

/* WARNING: Type propagation algorithm not settling */

void forced_0042a9a0(void)

{
  int iVar1;
  ssize_t sVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  uint32_t uVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  epoll_event aeStack_560 [5];
  undefined4 uStack_510;
  epoll_event aeStack_160 [10];
  char acStack_c0 [32];
  sockaddr sStack_a0;
  socklen_t asStack_90 [2];
  char *pcStack_88;
  char *pcStack_84;
  uint32_t uStack_80;
  char *pcStack_7c;
  char *pcStack_78;
  char *pcStack_74;
  char *pcStack_70;
  char *pcStack_6c;
  char *pcStack_68;
  char *pcStack_64;
  undefined *puStack_60;
  char *pcStack_5c;
  char *pcStack_58;
  char *pcStack_54;
  char *pcStack_50;
  char *pcStack_4c;
  char *pcStack_48;
  char *pcStack_44;
  char *pcStack_40;
  char *pcStack_3c;
  char *pcStack_38;
  char *pcStack_34;
  char *pcStack_30;
  char *pcStack_2c;
  
  asStack_90[0] = 0x10;
  pcStack_7c = "nd bgm path";
  pcStack_6c = "nd bgm path";
  pcStack_68 = "nd bgm path";
  pcStack_64 = "nd bgm path";
  pcStack_58 = "nd bgm path";
  pcStack_88 = "nd bgm path";
  pcStack_84 = "nd bgm path";
  pcStack_78 = "nd bgm path";
  pcStack_5c = "nd bgm path";
  pcStack_50 = "nd bgm path";
  pcStack_4c = "nd bgm path";
  pcStack_54 = "nd bgm path";
  pcStack_34 = "nd bgm path";
  pcStack_30 = "nd bgm path";
  puStack_60 = &DAT_01210000;
  pcStack_2c = "nd bgm path";
  pcStack_40 = "nd bgm path";
  pcStack_3c = "nd bgm path";
  pcStack_38 = "nd bgm path";
  pcStack_70 = "nd bgm path";
  pcStack_44 = "nd bgm path";
  pcStack_48 = "nd bgm path";
  pcStack_74 = "nd bgm path";
  uStack_80 = 0x40601c0a;
  do {
    do {
      while (iVar1 = epoll_wait(DAT_0121d0ec,aeStack_160,10,1000), iVar1 == -1) {
        piVar11 = __errno_location();
        if (*piVar11 != 4) {
          usleep(100000);
        }
      }
    } while (iVar1 == 0);
    if (0 < iVar1) {
      iVar10 = 0;
      puVar9 = (undefined4 *)((int)aeStack_160 + 8);
      do {
        while( true ) {
          piVar11 = (int *)*puVar9;
          uVar7 = piVar11[1];
          iVar12 = *piVar11;
          if (uVar7 == 1) break;
          if (uVar7 == 0) {
            memset(aeStack_560,0,0x400);
            sVar2 = read(iVar12,aeStack_560,0x400);
            if (0 < sVar2) {
              if (aeStack_560[0].events == 0x20001c09) {
                printf(pcStack_68 + 0x1880,pcStack_7c + 0x22fc,0x2d7);
              }
              else if (aeStack_560[0].events == uStack_80) {
                printf(pcStack_64 + 0x18a4,pcStack_7c + 0x22fc,uStack_510);
              }
              else if (aeStack_560[0].events == 0x20001c08) {
                printf(pcStack_6c + 0x185c,pcStack_7c + 0x22fc,0x2d3);
              }
            }
          }
          else if (uVar7 == 2) {
            printf(pcStack_88 + 0x1900,"hotplug_receive_event_func",0x329);
            memset(aeStack_560,0,0x80);
            sVar2 = read(iVar12,aeStack_560,0x7f);
            if (sVar2 < 1) {
              if (sVar2 == 0) {
                puts(pcStack_70 + 0x19b8);
              }
              else {
                piVar5 = __errno_location();
                iVar4 = *piVar5;
                if ((iVar4 == 4) || (iVar4 == 0xb)) {
                  pcVar3 = strerror(iVar4);
                  printf(pcStack_48 + 0x19e0,pcVar3);
                }
                else {
                  perror(pcStack_44 + 0x19d4);
                }
              }
            }
            else {
              puts((char *)aeStack_560);
              pcVar3 = strstr((char *)aeStack_560,pcStack_84 + 0x1924);
              if ((pcVar3 != (char *)0x0) &&
                 (pcVar3 = strstr((char *)aeStack_560,pcStack_78 + 0x192c), pcVar3 == (char *)0x0))
              {
                pcVar3 = strstr((char *)aeStack_560,pcStack_5c + 0x1938);
                if (pcVar3 == (char *)0x0) {
                  pcVar3 = strstr((char *)aeStack_560,pcStack_40 + 0x1978);
                  if (pcVar3 == (char *)0x0) goto LAB_0042ab60;
                  __isoc99_sscanf(aeStack_560,pcStack_3c + 0x1988,acStack_c0);
                  printf(pcStack_38 + 0x19a0,acStack_c0);
                  pcVar3 = strstr(acStack_c0,pcStack_54 + -0x110);
                  if (((pcVar3 == (char *)0x0) &&
                      (pcVar3 = strstr(acStack_c0,pcStack_34 + -0x10c), pcVar3 == (char *)0x0)) &&
                     (pcVar3 = strstr(acStack_c0,pcStack_30 + -0x108), pcVar3 == (char *)0x0)) {
                    pcVar3 = strstr(acStack_c0,pcStack_2c + -0x5d4);
                    if (pcVar3 == (char *)0x0) goto LAB_0042af14;
                    uVar8 = 8;
                    *(undefined4 *)(puStack_60 + 0x46d0) = 8;
                  }
                  else {
                    uVar8 = 1;
                    *(undefined4 *)(puStack_60 + 0x46d0) = 1;
                  }
                }
                else {
                  __isoc99_sscanf(aeStack_560,pcStack_50 + 0x1948,acStack_c0);
                  printf(pcStack_4c + 0x1960,acStack_c0);
                  pcVar3 = strstr(acStack_c0,pcStack_54 + -0x110);
                  if (((pcVar3 == (char *)0x0) &&
                      (pcVar3 = strstr(acStack_c0,pcStack_34 + -0x10c), pcVar3 == (char *)0x0)) &&
                     (pcVar3 = strstr(acStack_c0,pcStack_30 + -0x108), pcVar3 == (char *)0x0)) {
                    pcVar3 = strstr(acStack_c0,pcStack_2c + -0x5d4);
                    if (pcVar3 == (char *)0x0) {
LAB_0042af14:
                      uVar8 = *(undefined4 *)(puStack_60 + 0x46d0);
                    }
                    else {
                      uVar8 = 7;
                      *(undefined4 *)(puStack_60 + 0x46d0) = 7;
                    }
                  }
                  else {
                    uVar8 = 0;
                    *(undefined4 *)(puStack_60 + 0x46d0) = 0;
                  }
                }
                uVar6 = __strdup(acStack_c0);
                FUN_0042a8b4(uVar8,uVar6);
              }
            }
LAB_0042ab60:
            iVar4 = epoll_ctl(DAT_0121d0ec,2,iVar12,(epoll_event *)0x0);
            if (iVar4 == -1) {
              perror(pcStack_74 + 0x19fc);
            }
            close(iVar12);
            free(piVar11);
          }
LAB_0042ab90:
          iVar10 = iVar10 + 1;
          puVar9 = puVar9 + 4;
          if (iVar1 == iVar10) goto LAB_0042ac3c;
        }
        printf("%s(), line: %d. get hotplug connect...\n","hotplug_receive_event_func",0x314);
        iVar12 = accept(iVar12,&sStack_a0,asStack_90);
        if (iVar12 < 0) goto LAB_0042ab90;
        piVar11 = malloc(8);
        iVar4 = DAT_0121d0ec;
        *piVar11 = iVar12;
        piVar11[1] = 2;
        aeStack_560[0].events = uVar7;
        aeStack_560[0].data._0_4_ = piVar11;
        iVar4 = epoll_ctl(iVar4,1,iVar12,aeStack_560);
        if (iVar4 != -1) goto LAB_0042ab90;
        iVar10 = iVar10 + 1;
        puVar9 = puVar9 + 4;
        perror(pcStack_58 + 0x18e4);
        close(iVar12);
        free(piVar11);
      } while (iVar1 != iVar10);
    }
LAB_0042ac3c:
    usleep(10000);
  } while( true );
}



/* ===== FORCED FUNCTION forced_0042af20 @ 0042af20 ===== */

void forced_0042af20(uint param_1)

{
  bool bVar1;
  int iVar2;
  DIR *__dirp;
  dirent *pdVar3;
  char *pcVar4;
  undefined4 uVar5;
  char acStack_230 [512];
  char *pcStack_30;
  char *pcStack_2c;
  
  if (DAT_012146d0 != 0) {
    __dirp = opendir("/media");
    if (__dirp != (DIR *)0x0) {
      param_1 = param_1 / 100;
      if (param_1 != 0) {
        pcStack_30 = "hd";
        bVar1 = false;
        pcStack_2c = "nd bgm path";
LAB_0042afe8:
        do {
          pdVar3 = readdir(__dirp);
          while (pdVar3 != (dirent *)0x0) {
            if ((((pdVar3->d_name[0] == '.') &&
                 ((pdVar3->d_name[1] == '\0' ||
                  ((pdVar3->d_name[1] == '.' && (pdVar3->d_name[2] == '\0')))))) ||
                (pdVar3->d_name[0] == '\0')) || (pdVar3->d_type != '\x04')) goto LAB_0042afe8;
            sprintf(acStack_230,"%s/%s","/media",pdVar3->d_name);
            pcVar4 = strstr(acStack_230,"sd");
            if (((pcVar4 == (char *)0x0) &&
                (pcVar4 = strstr(acStack_230,pcStack_30), pcVar4 == (char *)0x0)) &&
               (pcVar4 = strstr(acStack_230,pcStack_2c + -0x108), pcVar4 == (char *)0x0)) {
              pcVar4 = strstr(acStack_230,(char *)&PTR_DAT_0044fa2c);
              if (pcVar4 != (char *)0x0) {
                DAT_012146d0 = 7;
              }
            }
            else {
              DAT_012146d0 = 0;
            }
            iVar2 = DAT_012146d0;
            uVar5 = __strdup(acStack_230);
            FUN_0042a8b4(iVar2,uVar5);
            bVar1 = true;
            printf("%s(), line: %d. found USB device: %s!\n","dev_ready_check",0xa78,acStack_230);
            pdVar3 = readdir(__dirp);
          }
          param_1 = param_1 - 1;
          usleep(100000);
        } while (param_1 != 0);
        if (bVar1) {
          DAT_012146d0 = 0;
        }
      }
      printf("%s(), line: %d. No USB device!\n","dev_ready_check",0xa83);
      closedir(__dirp);
      return;
    }
    printf("%s(), line: %d. open dir:%s error!\n","dev_ready_check",0xa5c,"/media");
  }
  return;
}



/* ===== FORCED FUNCTION forced_00427414 @ 00427414 ===== */

void forced_00427414(void)

{
  void *__ptr;
  int iVar1;
  int iVar2;
  void *apvStack_38 [2];
  char *pcStack_30;
  
  apvStack_38[0] = (void *)0x0;
  pcStack_30 = "nd bgm path";
  do {
    do {
      do {
        sem_wait((sem_t *)&DAT_0121ed4c);
      } while (DAT_012146b4 == -1);
      iVar1 = FUN_0042a720(DAT_012146b4,apvStack_38,4,5);
      __ptr = apvStack_38[0];
    } while ((iVar1 != 0) || (apvStack_38[0] == (void *)0x0));
    FUN_0042eee8();
    iVar1 = open("/dev/persistentmem",2);
    if (iVar1 < 0) {
      FUN_0042eef4(iVar1,0x800c2603,__ptr);
      printf("%s(), line:%d. Open %s fail!\n","projector_memory_save_task",0x5e8,
             "/dev/persistentmem");
    }
    else {
      iVar2 = ioctl(iVar1,0x800c2603);
      if (iVar2 < 0) {
        printf(pcStack_30 + 0x1234,"projector_memory_save_task",0x5ee);
      }
      FUN_0042eef4();
      close(iVar1);
    }
    free(*(void **)((int)__ptr + 8));
    free(__ptr);
  } while( true );
}



/* ===== FORCED FUNCTION forced_00420470 @ 00420470 ===== */

undefined4 forced_00420470(void)

{
  bool bVar1;
  int __fd;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  ssize_t sVar5;
  int iVar6;
  undefined4 uVar7;
  int unaff_s0;
  void *__ptr;
  int iVar8;
  undefined1 auStack_b0 [20];
  uint uStack_9c;
  byte bStack_96;
  byte bStack_95;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  uint uStack_40;
  undefined4 uStack_3c;
  undefined4 auStack_38 [2];
  uint uStack_30;
  undefined *puStack_2c;
  
  uStack_60 = 0;
  uStack_5c = 0;
  uStack_58 = 0;
  uStack_54 = 0;
  uStack_50 = 0;
  uStack_4c = 0;
  auStack_38[0] = 10;
  memset(auStack_b0,0,0x50);
  __fd = open("/dev/sndC0i2so",1);
  if (__fd < 0) {
    puts("error: can not open i2so dev ");
    puts("i2so rec exit");
  }
  else {
    uVar2 = ioctl(__fd,0x20000811,0x4b000);
    printf("ret0 %d\n",uVar2);
    uVar3 = ioctl(__fd,0x40180500,&uStack_60);
    unaff_s0 = open("/dev/kshmdev",0);
    if (-1 < unaff_s0) {
      uVar4 = ioctl(unaff_s0,0x40180501,&uStack_60);
      uVar4 = uVar2 | uVar3 | uVar4;
      printf("ret2 %d\n",uVar4);
      if (uVar4 == 0) {
        puts("i2so rec start");
        iVar8 = 0;
        bVar1 = true;
        puStack_2c = &DAT_01210000;
        __ptr = (void *)0x0;
        uVar2 = 0;
        while (DAT_01217ca8 == '\0') {
          while( true ) {
            uStack_48 = 0;
            uStack_44 = 0;
            uStack_40 = 0;
            uStack_3c = 0;
            while ((sVar5 = read(unaff_s0,&uStack_48,0x10), sVar5 != 0x10 && (DAT_01217ca8 == '\0'))
                  ) {
              usleep(10000);
            }
            uVar3 = uStack_40 & 0x3fffffff;
            if (((int)uVar2 < (int)uVar3) &&
               (uStack_30 = uVar3, __ptr = realloc(__ptr,uVar3), uVar3 = uStack_30,
               uVar2 = uStack_30, __ptr == (void *)0x0)) {
              puts("no memory");
              goto LAB_00420720;
            }
            while ((uVar3 = read(unaff_s0,__ptr,uVar3), uVar3 != (uStack_40 & 0x3fffffff) &&
                   (DAT_01217ca8 == '\0'))) {
              usleep(20000);
              uVar3 = uStack_40 & 0x3fffffff;
            }
            iVar6 = ioctl(__fd,0x40500810,auStack_b0);
            if ((((iVar6 != 0) || (bStack_96 == 0)) || (bStack_95 == 0)) || (uStack_9c == 0)) break;
            uVar7 = 0x18;
            uStack_30 = 0x10;
            if (bStack_95 == 0x10) {
              uVar7 = 0x10;
            }
            uVar7 = snd_spectrum_run(__ptr,bStack_96,uVar7,uStack_9c,uStack_40 & 0x3fffffff,
                                     auStack_38);
            *(undefined4 *)(puStack_2c + 0x7ca0) = uVar7;
            uVar3 = (uint)bStack_96;
            if (bStack_95 == uStack_30) {
              if (uVar3 == 0) {
                trap(7);
              }
              iVar6 = (int)((uStack_40 & 0x3fffffff) / uVar3) >> 1;
            }
            else {
              if (uVar3 == 0) {
                trap(7);
              }
              iVar6 = (int)((uStack_40 & 0x3fffffff) / uVar3) >> 2;
            }
            if (uStack_9c == 0) {
              trap(7);
            }
            iVar8 = (uint)(iVar6 * 1000) / uStack_9c + iVar8;
            if ((bVar1) || (99 < iVar8)) {
              iVar8 = 0;
              FUN_0040ec74();
            }
            bVar1 = false;
            usleep(8000);
            if (DAT_01217ca8 != '\0') goto LAB_00420720;
          }
        }
LAB_00420720:
        ioctl(__fd,0x20000812,0);
        snd_spectrum_stop();
        puts("i2so rec exit");
        close(__fd);
        if (__ptr != (void *)0x0) {
          free(__ptr);
        }
        goto LAB_00420794;
      }
      printf("set i2so rec failed %d\n",uVar4);
      ioctl(__fd,0x20000812,0);
    }
    puts("i2so rec exit");
    close(__fd);
  }
LAB_00420794:
  if (0 < unaff_s0) {
    close(unaff_s0);
  }
  return 0;
}



/* ===== FORCED FUNCTION forced_004220f0 @ 004220f0 ===== */

undefined4 forced_004220f0(void)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00423b88();
  FUN_00421f58(uVar1);
  return 0;
}



/* ===== FORCED FUNCTION forced_0044d554 @ 0044d554 ===== */

undefined4 forced_0044d554(void)

{
  int iVar1;
  key_t __key;
  undefined4 *puVar2;
  undefined4 uVar3;
  stat sStack_a0;
  
  mkdir("/tmp/joy_key",0x1ff);
  iVar1 = __xstat(3,"/tmp/joy_key",&sStack_a0);
  if ((iVar1 == 0) && ((sStack_a0.st_nlink & 0xf000) == 0x4000)) {
    printf("mkdir \"%s\" success\n","/tmp/joy_key");
    __key = ftok("/tmp/joy_key",0x61);
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
        if (puVar2 != (undefined4 *)0x0) {
          printf("%s %d %d\n","cube_shmget_joy_key_init",0xc6,*puVar2);
          *puVar2 = 0;
          uVar3 = 0;
          do {
            FUN_0044d390(uVar3);
            usleep(16000);
            uVar3 = *puVar2;
          } while( true );
        }
        perror("shmat");
      }
    }
  }
  else {
    perror("mkdir");
  }
  puts("cube_shmget_joy_key_init error");
  return 0;
}


