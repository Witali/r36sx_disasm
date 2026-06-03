
/* ===== FUNCTION _init @ 00400644 ===== */

int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  __gmon_start__();
  frame_dummy();
  iVar1 = __do_global_ctors_aux();
  return iVar1;
}



/* ===== FUNCTION main @ 004006b0 ===== */

undefined4 main(void)

{
  undefined4 *puVar1;
  __pid_t _Var2;
  int iVar3;
  undefined4 *__s;
  int iVar4;
  
  puts("\nicube v1.3");
  ShareMemCreat();
  puVar1 = shm;
  __s = shm + 0x56;
  shm[2] = 0xffffffff;
  puVar1[0x49] = 0xfffffffe;
  puVar1[0x4a] = 0xfffffffe;
  puVar1[0x4b] = 0xfffffffe;
  puVar1[0x4c] = 0xfffffffe;
  puVar1[0x4d] = 0xfffffffe;
  puVar1[0x4e] = 0xfffffffe;
  puVar1[0x4f] = 0xfffffffe;
  puVar1[0x50] = 0xfffffffe;
  puVar1[0x51] = 0xfffffffe;
  puVar1[0x52] = 0xfffffffe;
  puVar1[0x53] = 0xfffffffe;
  puVar1[0x54] = 0xfffffffe;
  puVar1[0x55] = 0xfffffffe;
  memset(__s,0,100);
  puVar1[0x6f] = 0xfffffffe;
  puVar1[0x70] = 0xfffffffe;
  _Var2 = fork();
  if (_Var2 < 0) {
    perror("fork error!\n");
  }
  else if (_Var2 == 0) {
LAB_004007c8:
    execl("/mnt/sdcard/cubegm/rkgame","rkgame",0);
    return 0;
  }
  sleep(6);
  iVar4 = 0;
  do {
    iVar3 = shm[1];
    *shm = 0;
    if ((iVar4 == iVar3) || (iVar3 == 0)) {
      system("killall rkgame");
      _Var2 = fork();
      if (_Var2 < 0) {
        perror("fork error!\n");
      }
      else if (_Var2 == 0) goto LAB_004007c8;
      iVar3 = 0;
      sleep(6);
    }
    sleep(1);
    iVar4 = iVar3;
  } while( true );
}



/* ===== FUNCTION __start @ 00400820 ===== */

void processEntry __start(undefined4 param_1,undefined4 param_2)

{
  undefined1 auStack_20 [16];
  code *local_10;
  undefined4 local_c;
  undefined1 *local_8;
  
  local_8 = auStack_20;
  local_10 = __libc_csu_fini;
  local_c = param_1;
  __libc_start_main(main,param_2,&stack0x00000004,__libc_csu_init);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION deregister_tm_clones @ 00400870 ===== */

/* WARNING: Removing unreachable block (ram,0x00400890) */
/* WARNING: Removing unreachable block (ram,0x0040089c) */

void deregister_tm_clones(void)

{
  return;
}



/* ===== FUNCTION register_tm_clones @ 004008ac ===== */

/* WARNING: Removing unreachable block (ram,0x004008d8) */
/* WARNING: Removing unreachable block (ram,0x004008e4) */

void register_tm_clones(void)

{
  return;
}



/* ===== FUNCTION __do_global_dtors_aux @ 004008f4 ===== */

/* WARNING: Removing unreachable block (ram,0x00400950) */

void __do_global_dtors_aux(void)

{
  if (completed_6286 == '\0') {
    deregister_tm_clones();
    completed_6286 = '\x01';
  }
  return;
}



/* ===== FUNCTION frame_dummy @ 004009a4 ===== */

/* WARNING: Removing unreachable block (ram,0x004009cc) */

void frame_dummy(void)

{
  register_tm_clones();
  return;
}



/* ===== FUNCTION ShareMemCreat @ 004009f0 ===== */

int ShareMemCreat(void)

{
  int iVar1;
  
  shmid = shmget(0x4d2,0x1c4,0x3b6);
  if (shmid == -1) {
    puts("shmget failed");
    return -1;
  }
  shm = shmat(shmid,(void *)0x0,0);
  if (shm != (void *)0xffffffff) {
    iVar1 = printf("\nMemory attached at %X\n",shm);
    return iVar1;
  }
  puts("shmat failed");
  return -1;
}



/* ===== FUNCTION ShareMemClose @ 00400a94 ===== */

undefined4 ShareMemClose(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = shmdt(shm);
  if (iVar1 == -1) {
    puts("shmdt failed");
    uVar2 = 0xffffffff;
  }
  else {
    iVar1 = shmctl(shmid,0,(shmid_ds *)0x0);
    if (iVar1 == -1) {
      puts("shmctl(IPC_RMID) failed");
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}



/* ===== FUNCTION get_old_file_time @ 00400b1c ===== */

undefined4 get_old_file_time(void)

{
  undefined4 *puVar1;
  
  puVar1 = shm + 1;
  *shm = 0;
  return *puVar1;
}



/* ===== FUNCTION wait4children @ 00400b30 ===== */

void wait4children(void)

{
  undefined1 auStack_10 [12];
  
  wait(auStack_10);
  return;
}



/* ===== FUNCTION read_childproc @ 00400b4c ===== */

void read_childproc(void)

{
  __pid_t _Var1;
  uint local_10 [3];
  
  _Var1 = waitpid(-1,(int *)local_10,1);
  if ((local_10[0] & 0x7f) == 0) {
    printf("Removed proc id: %d \n",_Var1);
    printf("Child send: %d \n",local_10[0] >> 8 & 0xff);
  }
  return;
}



/* ===== FUNCTION __libc_csu_init @ 00400ba0 ===== */

/* WARNING: Removing unreachable block (ram,0x00400c00) */

void __libc_csu_init(int argc,char **argv,char **envp)

{
                    /* Unresolved local var: size_t size@[???] */
  _init((EVP_PKEY_CTX *)argc);
                    /* Unresolved local var: size_t i@[???] */
  return;
}



/* ===== FUNCTION __libc_csu_fini @ 00400c44 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void __libc_csu_fini(void)

{
  return;
}



/* ===== FUNCTION __do_global_ctors_aux @ 00400c50 ===== */

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



/* ===== FUNCTION _fini @ 00400cb0 ===== */

void _fini(void)

{
  __do_global_dtors_aux();
  return;
}



/* ===== FUNCTION _PROCEDURE_LINKAGE_TABLE_ @ 00400de0 ===== */

void _PROCEDURE_LINKAGE_TABLE_(char *param_1)

{
  (*(code *)(undefined *)0x0)();
  printf(param_1);
  return;
}



/* ===== FUNCTION printf @ 00400e00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

{
  int iVar1;
  
  iVar1 = printf(__format);
  return iVar1;
}



/* ===== FUNCTION shmget @ 00400e10 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmget(key_t __key,size_t __size,int __shmflg)

{
  int iVar1;
  
  iVar1 = shmget(__key,__size,__shmflg);
  return iVar1;
}



/* ===== FUNCTION sleep @ 00400e20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uint sleep(uint __seconds)

{
  uint uVar1;
  
  uVar1 = sleep(__seconds);
  return uVar1;
}



/* ===== FUNCTION wait @ 00400e30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t wait(void *__stat_loc)

{
  __pid_t _Var1;
  
  _Var1 = wait(__stat_loc);
  return _Var1;
}



/* ===== FUNCTION shmat @ 00400e40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * shmat(int __shmid,void *__shmaddr,int __shmflg)

{
  void *pvVar1;
  
  pvVar1 = shmat(__shmid,__shmaddr,__shmflg);
  return pvVar1;
}



/* ===== FUNCTION perror @ 00400e50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void perror(char *__s)

{
  perror(__s);
  return;
}



/* ===== FUNCTION waitpid @ 00400e60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t waitpid(__pid_t __pid,int *__stat_loc,int __options)

{
  __pid_t _Var1;
  
  _Var1 = waitpid(__pid,__stat_loc,__options);
  return _Var1;
}



/* ===== FUNCTION shmctl @ 00400e70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmctl(int __shmid,int __cmd,shmid_ds *__buf)

{
  int iVar1;
  
  iVar1 = shmctl(__shmid,__cmd,__buf);
  return iVar1;
}



/* ===== FUNCTION puts @ 00400e80 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int puts(char *__s)

{
  int iVar1;
  
  iVar1 = puts(__s);
  return iVar1;
}



/* ===== FUNCTION system @ 00400e90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

{
  int iVar1;
  
  iVar1 = system(__command);
  return iVar1;
}



/* ===== FUNCTION __libc_start_main @ 00400ea0 ===== */

void __libc_start_main(void)

{
  __libc_start_main();
  return;
}



/* ===== FUNCTION memset @ 00400eb0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memset(__s,__c,__n);
  return pvVar1;
}



/* ===== FUNCTION execl @ 00400ec0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int execl(char *__path,char *__arg,...)

{
  int iVar1;
  
  iVar1 = execl(__path,__arg);
  return iVar1;
}



/* ===== FUNCTION shmdt @ 00400ed0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmdt(void *__shmaddr)

{
  int iVar1;
  
  iVar1 = shmdt(__shmaddr);
  return iVar1;
}



/* ===== FUNCTION fork @ 00400ee0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t fork(void)

{
  __pid_t _Var1;
  
  _Var1 = fork();
  return _Var1;
}



/* ===== FUNCTION printf @ 00411000 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* printf@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION shmget @ 00411004 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmget(key_t __key,size_t __size,int __shmflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* shmget@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION sleep @ 00411008 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

uint sleep(uint __seconds)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sleep@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION wait @ 0041100c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t wait(void *__stat_loc)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* wait@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION shmat @ 00411010 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * shmat(int __shmid,void *__shmaddr,int __shmflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* shmat@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION perror @ 00411014 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void perror(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* perror@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION waitpid @ 00411018 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t waitpid(__pid_t __pid,int *__stat_loc,int __options)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* waitpid@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION shmctl @ 0041101c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmctl(int __shmid,int __cmd,shmid_ds *__buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* shmctl@@GLIBC_2.2 */
  halt_baddata();
}



/* ===== FUNCTION puts @ 00411020 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int puts(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* puts@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION system @ 00411024 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* system@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION __libc_start_main @ 00411028 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __libc_start_main(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __libc_start_main@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION memset @ 0041102c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memset@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION execl @ 00411030 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int execl(char *__path,char *__arg,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* execl@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION shmdt @ 00411034 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmdt(void *__shmaddr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* shmdt@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION fork @ 00411038 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t fork(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fork@@GLIBC_2.0 */
  halt_baddata();
}



/* ===== FUNCTION __gmon_start__ @ 0041103c ===== */

/* WARNING: Control flow encountered bad instruction data */

void __gmon_start__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


