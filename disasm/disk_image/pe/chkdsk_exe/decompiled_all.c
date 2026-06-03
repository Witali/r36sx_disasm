
/* ===== FUNCTION __security_check_cookie @ 140001010 ===== */

/* WARNING: This is an inlined function */
/* Library Function - Single Match
    __security_check_cookie
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl __security_check_cookie(uintptr_t _StackCookie)

{
  if ((_StackCookie == DAT_140008000) && ((short)(_StackCookie >> 0x30) == 0)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __report_gsfailure(_StackCookie);
}



/* ===== FUNCTION __report_gsfailure @ 140001040 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __report_gsfailure
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl __report_gsfailure(uintptr_t _StackCookie)

{
  DWORD64 ControlPc;
  PRUNTIME_FUNCTION FunctionEntry;
  DWORD64 unaff_retaddr;
  uintptr_t local_res8 [4];
  DWORD64 local_38;
  ulonglong local_30;
  PVOID local_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  local_res8[0] = _StackCookie;
  RtlCaptureContext(&DAT_1400080c0);
  ControlPc = DAT_1400081b8;
  FunctionEntry = RtlLookupFunctionEntry(DAT_1400081b8,&local_38,(PUNWIND_HISTORY_TABLE)0x0);
  if (FunctionEntry == (PRUNTIME_FUNCTION)0x0) {
    _DAT_140008158 = local_res8;
    DAT_1400081b8 = unaff_retaddr;
  }
  else {
    RtlVirtualUnwind(0,local_38,ControlPc,FunctionEntry,(PCONTEXT)&DAT_1400080c0,&local_28,&local_30
                     ,(PKNONVOLATILE_CONTEXT_POINTERS)0x0);
  }
  _DAT_140008030 = DAT_1400081b8;
  _DAT_140008140 = local_res8[0];
  _DAT_140008020 = 0xc0000409;
  _DAT_140008024 = 1;
  _DAT_140008038 = 1;
  DAT_140008040 = 2;
  uStack_20 = DAT_140008000;
  uStack_18 = DAT_140008008;
  RtlUnhandledExceptionFilter(&PTR_DAT_140005000);
  NtTerminateProcess(0xffffffffffffffff,0xc0000409);
  return;
}



/* ===== FUNCTION __security_init_cookie @ 140001190 ===== */

/* Library Function - Single Match
    __security_init_cookie
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl __security_init_cookie(void)

{
  if ((DAT_140008000 == 0) || (DAT_140008000 == 0x2b992ddfa232)) {
    FUN_1400011dc(&DAT_140008000);
  }
  DAT_140008008 = ~DAT_140008000;
  return;
}



/* ===== FUNCTION FUN_1400011dc @ 1400011dc ===== */

void FUN_1400011dc(ulonglong *param_1)

{
  ulonglong uVar1;
  undefined8 in_RAX;
  
  uVar1 = rdtsc();
  *param_1 = ((CONCAT44((int)((ulonglong)in_RAX >> 0x20),(int)uVar1) | uVar1 & 0xffffffff00000000)
              >> 4 ^ (ulonglong)param_1) & 0xffffffffffff;
  if ((*param_1 == 0) || (*param_1 == 0x2b992ddfa232)) {
    *param_1 = 0x2b992ddfa233;
  }
  return;
}



/* ===== FUNCTION NtTerminateProcess @ 140001240 ===== */

void NtTerminateProcess(void)

{
                    /* WARNING: Could not recover jumptable at 0x000140001240. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  NtTerminateProcess();
  return;
}



/* ===== FUNCTION RtlCaptureContext @ 14000124c ===== */

void RtlCaptureContext(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000124c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  RtlCaptureContext();
  return;
}



/* ===== FUNCTION RtlLookupFunctionEntry @ 140001258 ===== */

PRUNTIME_FUNCTION __stdcall
RtlLookupFunctionEntry(DWORD64 ControlPc,PDWORD64 ImageBase,PUNWIND_HISTORY_TABLE HistoryTable)

{
  PRUNTIME_FUNCTION p_Var1;
  
                    /* WARNING: Could not recover jumptable at 0x000140001258. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  p_Var1 = RtlLookupFunctionEntry(ControlPc,ImageBase,HistoryTable);
  return p_Var1;
}



/* ===== FUNCTION RtlVirtualUnwind @ 140001264 ===== */

PEXCEPTION_ROUTINE __stdcall
RtlVirtualUnwind(DWORD HandlerType,DWORD64 ImageBase,DWORD64 ControlPc,
                PRUNTIME_FUNCTION FunctionEntry,PCONTEXT ContextRecord,PVOID *HandlerData,
                PDWORD64 EstablisherFrame,PKNONVOLATILE_CONTEXT_POINTERS ContextPointers)

{
  PEXCEPTION_ROUTINE puVar1;
  
                    /* WARNING: Could not recover jumptable at 0x000140001264. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  puVar1 = RtlVirtualUnwind(HandlerType,ImageBase,ControlPc,FunctionEntry,ContextRecord,HandlerData,
                            EstablisherFrame,ContextPointers);
  return puVar1;
}



/* ===== FUNCTION RtlUnhandledExceptionFilter @ 140001270 ===== */

void RtlUnhandledExceptionFilter(void)

{
                    /* WARNING: Could not recover jumptable at 0x000140001270. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  RtlUnhandledExceptionFilter();
  return;
}



/* ===== FUNCTION FUN_140001280 @ 140001280 ===== */

/* WARNING: Removing unreachable block (ram,0x0001400012d8) */
/* WARNING: Removing unreachable block (ram,0x000140001292) */
/* WARNING: Removing unreachable block (ram,0x0001400012e0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_140001280(void)

{
  int iVar1;
  
  DAT_1400085b0 = 0;
  iVar1 = FUN_140001698(1);
  __set_app_type(iVar1);
  _DAT_1400085c8 = 0xffffffffffffffff;
  _DAT_1400085d0 = 0xffffffffffffffff;
  *(undefined4 *)_fmode_exref = DAT_1400085c4;
  *(undefined4 *)_commode_exref = DAT_1400085b8;
  FUN_1400016f0();
  if (DAT_140008010 == 0) {
    __setusermatherr(FUN_1400017f0);
  }
  return 0;
}



/* ===== FUNCTION FUN_140001360 @ 140001360 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140001360(void)

{
  _DAT_1400085b4 = DAT_1400085c0;
  _DAT_14000859c =
       __getmainargs(&DAT_140008598,&DAT_1400085a0,&DAT_1400085a8,DAT_1400085bc,&DAT_1400085b4);
  return;
}



/* ===== FUNCTION FUN_1400013b0 @ 1400013b0 ===== */

/* WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall */

uint FUN_1400013b0(void)

{
  void *pvVar1;
  bool bVar2;
  void *pvVar3;
  int iVar4;
  BOOL BVar5;
  void *pvVar6;
  longlong *plVar7;
  bool bVar8;
  
  pvVar1 = StackBase;
  bVar2 = false;
  while( true ) {
    pvVar6 = (void *)0x0;
    LOCK();
    bVar8 = DAT_1400085d8 == (void *)0x0;
    pvVar3 = pvVar1;
    if (!bVar8) {
      pvVar6 = DAT_1400085d8;
      pvVar3 = DAT_1400085d8;
    }
    DAT_1400085d8 = pvVar3;
    UNLOCK();
    if (bVar8) goto LAB_140001401;
    if (pvVar6 == pvVar1) break;
    Sleep(1000);
  }
  bVar2 = true;
LAB_140001401:
  if (DAT_1400085e0 == 1) {
    _amsg_exit(0x1f);
  }
  else if (DAT_1400085e0 == 0) {
    DAT_1400085e0 = 1;
    iVar4 = 0;
    for (plVar7 = &DAT_140005548; plVar7 < &DAT_140005560; plVar7 = plVar7 + 1) {
      if (iVar4 != 0) {
        return 0xff;
      }
      if (*plVar7 != 0) {
        iVar4 = (*(code *)*plVar7)(_guard_dispatch_icall);
      }
    }
    if (iVar4 != 0) {
      return 0xff;
    }
  }
  else {
    DAT_140008594 = 1;
  }
  if (DAT_1400085e0 == 1) {
    _initterm(&DAT_140005530,&DAT_140005540);
    DAT_1400085e0 = 2;
  }
  if (!bVar2) {
    LOCK();
    DAT_1400085d8 = (void *)0x0;
    UNLOCK();
  }
  if ((DAT_1400085e8 != 0) &&
     (BVar5 = _IsNonwritableInCurrentImage((PBYTE)&DAT_1400085e8), BVar5 != 0)) {
    (*(code *)DAT_1400085e8)(0,2,0,_guard_dispatch_icall);
  }
  DAT_140008590 = FUN_1400018f8();
  if (DAT_1400085b0 == 0) {
                    /* WARNING: Subroutine does not return */
    exit(DAT_140008590);
  }
  if (DAT_140008594 == 0) {
    _cexit();
  }
  return DAT_140008590;
}



/* ===== FUNCTION entry @ 140001590 ===== */

void entry(void)

{
  __security_init_cookie();
  FUN_1400013b0();
  return;
}



/* ===== FUNCTION FUN_1400015c0 @ 1400015c0 ===== */

undefined8 FUN_1400015c0(undefined8 *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (((*piVar1 == -0x1f928c9d) && (piVar1[6] == 4)) &&
     ((piVar1[8] + 0xe66cfae0U < 3 || (piVar1[8] == 0x1994000)))) {
                    /* WARNING: Subroutine does not return */
    terminate();
  }
  return 0;
}



/* ===== FUNCTION FUN_140001610 @ 140001610 ===== */

undefined8 FUN_140001610(void)

{
  SetUnhandledExceptionFilter(FUN_1400015c0);
  return 0;
}



/* ===== FUNCTION _XcptFilter @ 14000162e ===== */

int __cdecl _XcptFilter(ulong _ExceptionNum,_EXCEPTION_POINTERS *_ExceptionPtr)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000162e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = _XcptFilter(_ExceptionNum,_ExceptionPtr);
  return iVar1;
}



/* ===== FUNCTION FUN_14000163c @ 14000163c ===== */

int * FUN_14000163c(short *param_1)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = (int *)0x0;
  piVar2 = piVar1;
  if (((((longlong)param_1 - 1U < 0xfffffffffffffffe) && (*param_1 == 0x5a4d)) &&
      (-1 < *(int *)(param_1 + 0x1e))) &&
     ((*(uint *)(param_1 + 0x1e) < 0x10000000 &&
      (piVar2 = (int *)((longlong)*(int *)(param_1 + 0x1e) + (longlong)param_1),
      *(int *)((longlong)*(int *)(param_1 + 0x1e) + (longlong)param_1) != 0x4550)))) {
    piVar2 = piVar1;
  }
  return piVar2;
}



/* ===== FUNCTION FUN_140001698 @ 140001698 ===== */

undefined4 FUN_140001698(undefined4 param_1)

{
  HMODULE pHVar1;
  int *piVar2;
  
  pHVar1 = GetModuleHandleW((LPCWSTR)0x0);
  if ((pHVar1 != (HMODULE)0x0) && (piVar2 = FUN_14000163c((short *)pHVar1), piVar2 != (int *)0x0)) {
    if ((short)piVar2[0x17] == 2) {
      param_1 = 2;
    }
    else if ((short)piVar2[0x17] == 3) {
      param_1 = 1;
    }
  }
  return param_1;
}



/* ===== FUNCTION _amsg_exit @ 1400016e4 ===== */

void __cdecl _amsg_exit(int param_1)

{
                    /* WARNING: Could not recover jumptable at 0x0001400016e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  _amsg_exit(param_1);
  return;
}



/* ===== FUNCTION FUN_1400016f0 @ 1400016f0 ===== */

undefined8 FUN_1400016f0(void)

{
  return 0;
}



/* ===== FUNCTION _FindPESection @ 140001700 ===== */

/* Library Function - Single Match
    _FindPESection
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

PIMAGE_SECTION_HEADER __cdecl _FindPESection(PBYTE pImageBase,DWORD_PTR rva)

{
  int iVar1;
  PIMAGE_SECTION_HEADER p_Var2;
  uint uVar3;
  
  iVar1 = *(int *)(pImageBase + 0x3c);
  uVar3 = 0;
  p_Var2 = (PIMAGE_SECTION_HEADER)
           (pImageBase +
           (ulonglong)*(ushort *)(pImageBase + (longlong)iVar1 + 0x14) + 0x18 + (longlong)iVar1);
  if (*(ushort *)(pImageBase + (longlong)iVar1 + 6) != 0) {
    do {
      if ((p_Var2->VirtualAddress <= rva) &&
         (rva < (p_Var2->Misc).PhysicalAddress + p_Var2->VirtualAddress)) {
        return p_Var2;
      }
      uVar3 = uVar3 + 1;
      p_Var2 = p_Var2 + 1;
    } while (uVar3 < *(ushort *)(pImageBase + (longlong)iVar1 + 6));
  }
  return (PIMAGE_SECTION_HEADER)0x0;
}



/* ===== FUNCTION _IsNonwritableInCurrentImage @ 140001750 ===== */

/* Library Function - Single Match
    _IsNonwritableInCurrentImage
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

BOOL __cdecl _IsNonwritableInCurrentImage(PBYTE pTarget)

{
  bool bVar1;
  uint uVar2;
  undefined7 extraout_var;
  PIMAGE_SECTION_HEADER p_Var3;
  
  bVar1 = FUN_1400017b0((short *)&IMAGE_DOS_HEADER_140000000);
  uVar2 = 0;
  if ((int)CONCAT71(extraout_var,bVar1) != 0) {
    p_Var3 = _FindPESection((PBYTE)&IMAGE_DOS_HEADER_140000000,(DWORD_PTR)(pTarget + -0x140000000));
    uVar2 = 0;
    if (p_Var3 != (PIMAGE_SECTION_HEADER)0x0) {
      uVar2 = ~(p_Var3->Characteristics >> 0x1f) & 1;
    }
  }
  return uVar2;
}



/* ===== FUNCTION FUN_1400017b0 @ 1400017b0 ===== */

bool FUN_1400017b0(short *param_1)

{
  if ((*param_1 == 0x5a4d) &&
     (*(int *)((longlong)*(int *)(param_1 + 0x1e) + (longlong)param_1) == 0x4550)) {
    return (short)((int *)((longlong)*(int *)(param_1 + 0x1e) + (longlong)param_1))[6] == 0x20b;
  }
  return false;
}



/* ===== FUNCTION FUN_1400017f0 @ 1400017f0 ===== */

undefined8 FUN_1400017f0(void)

{
  return 0;
}



/* ===== FUNCTION _initterm @ 1400017f9 ===== */

void _initterm(void)

{
                    /* WARNING: Could not recover jumptable at 0x0001400017f9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  _initterm();
  return;
}



/* ===== FUNCTION _guard_check_icall @ 140001810 ===== */

void _guard_check_icall(void)

{
  return;
}



/* ===== FUNCTION FUN_140001828 @ 140001828 ===== */

void FUN_140001828(OBJECT *param_1)

{
  DSTRING::~DSTRING((DSTRING *)(param_1 + 0x20));
                    /* WARNING: Could not recover jumptable at 0x000140001849. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  OBJECT::~OBJECT(param_1);
  return;
}



/* ===== FUNCTION FUN_140001860 @ 140001860 ===== */

undefined8 FUN_140001860(int param_1)

{
  uchar uVar1;
  STREAM *pSVar2;
  STREAM *pSVar3;
  CHKDSK_MESSAGE local_f8 [240];
  
  if ((param_1 != 0) && (param_1 != 1)) {
    return 0;
  }
  CHKDSK_MESSAGE::CHKDSK_MESSAGE(local_f8);
  pSVar2 = (STREAM *)Get_Standard_Input_Stream();
  pSVar3 = (STREAM *)Get_Standard_Output_Stream();
  uVar1 = CHKDSK_MESSAGE::Initialize(local_f8,pSVar3,pSVar2,(STREAM *)0x0);
  if (uVar1 != '\0') {
    MESSAGE::DisplayMsg((MESSAGE *)local_f8,0x45b);
  }
                    /* WARNING: Subroutine does not return */
  ExitProcess(0x15);
}



/* ===== FUNCTION FUN_1400018f8 @ 1400018f8 ===== */

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall */

uint FUN_1400018f8(void)

{
  code *pcVar1;
  uchar uVar2;
  BOOL BVar3;
  uint uVar4;
  PATH_ANALYZE_CODE PVar5;
  UINT UVar6;
  DRIVE_TYPE DVar7;
  uint uVar8;
  long lVar9;
  undefined4 uVar10;
  DWORD DVar11;
  longlong lVar12;
  STREAM *pSVar13;
  STREAM *pSVar14;
  __uint64 _Var15;
  WSTRING *pWVar16;
  _func___int64 *p_Var17;
  undefined1 *puVar18;
  ushort *puVar19;
  HANDLE hEventLog;
  CHKDSK_MESSAGE *this;
  ulong uVar20;
  PATH *this_00;
  char cVar21;
  uint uVar22;
  char cVar23;
  undefined1 *puVar24;
  char cVar25;
  char cVar26;
  undefined8 uStack_20;
  
  uStack_20 = 0x14000191b;
  lVar12 = FUN_140004920();
  lVar12 = -lVar12;
  *(ulonglong *)(&stack0x000017b8 + lVar12) =
       DAT_140008000 ^ (ulonglong)(&stack0xffffffffffffffe8 + lVar12);
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000193f;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000468 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001953;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000088 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001967;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000428 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000197b;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000248 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000198f;
  DSTRING::DSTRING((DSTRING *)(&stack0x000000b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400019a3;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000338 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400019b7;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000178 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400019cb;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000218 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400019df;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000308 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400019f3;
  DSTRING::DSTRING((DSTRING *)(&stack0x000002d8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a07;
  DSTRING::DSTRING((DSTRING *)(&stack0x000002a8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a1b;
  DSTRING::DSTRING((DSTRING *)(&stack0x000003f8 + lVar12));
  *(undefined4 *)(&stack0x0000004c + lVar12) = 0;
  *(undefined4 *)(&stack0x0000016c + lVar12) = 1;
  *(undefined4 *)(&stack0x00000170 + lVar12) = 1;
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a49;
  ARGUMENT_LEXEMIZER::ARGUMENT_LEXEMIZER((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a5d;
  ARRAY::ARRAY((ARRAY *)(&stack0x00000110 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a6e;
  ARRAY::ARRAY((ARRAY *)(&stack0x00000058 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a82;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000d78 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001a96;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000c58 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001aaa;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000778 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001abe;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000bf8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ad2;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000d18 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ae6;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x000009b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001afa;
  LONG_ARGUMENT::LONG_ARGUMENT((LONG_ARGUMENT *)(&stack0x000005f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b0e;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000718 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b22;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000958 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b36;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x000006b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b4a;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000b98 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b5e;
  LONG_ARGUMENT::LONG_ARGUMENT((LONG_ARGUMENT *)(&stack0x000008f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b72;
  STRING_ARGUMENT::STRING_ARGUMENT((STRING_ARGUMENT *)(&stack0x00000dd8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b86;
  PATH_ARGUMENT::PATH_ARGUMENT((PATH_ARGUMENT *)(&stack0x00000e68 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001b9a;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000cb8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001bae;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000598 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001bc2;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000658 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001bd6;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000838 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001bea;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000898 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001bfe;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000b38 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c12;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000a18 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c26;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000ad8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c3a;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x000007d8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c4e;
  FLAG_ARGUMENT::FLAG_ARGUMENT((FLAG_ARGUMENT *)(&stack0x00000a78 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c62;
  CHKDSK_MESSAGE::CHKDSK_MESSAGE((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c76;
  PATH::PATH((PATH *)(&stack0x000012f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c8a;
  PATH::PATH((PATH *)(&stack0x00001558 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001c9e;
  DSTRING::DSTRING((DSTRING *)(&stack0x000001e8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001cb2;
  DSTRING::DSTRING((DSTRING *)(&stack0x000001b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001cc6;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000138 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001cda;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000278 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001cee;
  DSTRING::DSTRING((DSTRING *)(&stack0x000003c8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d02;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000398 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d16;
  DSTRING::DSTRING((DSTRING *)(&stack0x00000368 + lVar12));
  *(undefined8 *)(&stack0x000001a8 + lVar12) = 0;
  *(undefined4 *)(&stack0x00000080 + lVar12) = 0;
  *(undefined8 *)(&stack0x00000460 + lVar12) = 0;
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d41;
  HMEM::HMEM((HMEM *)(&stack0x000000e8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d57;
  HeapSetInformation((HANDLE)0x0,HeapEnableTerminationOnCorruption,(PVOID)0x0,0);
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d63;
  pSVar13 = (STREAM *)Get_Standard_Input_Stream();
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d72;
  pSVar14 = (STREAM *)Get_Standard_Output_Stream();
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001d8f;
  uVar2 = CHKDSK_MESSAGE::Initialize
                    ((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),pSVar14,pSVar13,(STREAM *)0x0);
  if (uVar2 == '\0') {
    uVar4 = 3;
  }
  else {
    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001dac;
    BVar3 = SetConsoleCtrlHandler(FUN_140001860,1);
    if (BVar3 == 0) {
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001dcd;
      MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0xbfc);
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001de1;
      HMEM::~HMEM((HMEM *)(&stack0x000000e8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001df5;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000368 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e09;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000398 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e1d;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000003c8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e31;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000278 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e45;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000138 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e59;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000001b8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e6d;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000001e8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e81;
      PATH::~PATH((PATH *)(&stack0x00001558 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001e95;
      PATH::~PATH((PATH *)(&stack0x000012f8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ea9;
      CHKDSK_MESSAGE::~CHKDSK_MESSAGE((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ebb;
      FUN_140001828((OBJECT *)(&stack0x00000a78 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ec8;
      FUN_140001828((OBJECT *)(&stack0x000007d8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ed5;
      FUN_140001828((OBJECT *)(&stack0x00000ad8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ee2;
      FUN_140001828((OBJECT *)(&stack0x00000a18 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001eef;
      FUN_140001828((OBJECT *)(&stack0x00000b38 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001efc;
      FUN_140001828((OBJECT *)(&stack0x00000898 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f09;
      FUN_140001828((OBJECT *)(&stack0x00000838 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f16;
      FUN_140001828((OBJECT *)(&stack0x00000658 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f23;
      FUN_140001828((OBJECT *)(&stack0x00000598 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f30;
      FUN_140001828((OBJECT *)(&stack0x00000cb8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f3f;
      PATH_ARGUMENT::~PATH_ARGUMENT((PATH_ARGUMENT *)(&stack0x00000e68 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f53;
      STRING_ARGUMENT::~STRING_ARGUMENT((STRING_ARGUMENT *)(&stack0x00000dd8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f65;
      FUN_140001828((OBJECT *)(&stack0x000008f8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f72;
      FUN_140001828((OBJECT *)(&stack0x00000b98 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f7f;
      FUN_140001828((OBJECT *)(&stack0x000006b8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f8c;
      FUN_140001828((OBJECT *)(&stack0x00000958 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001f99;
      FUN_140001828((OBJECT *)(&stack0x00000718 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fa6;
      FUN_140001828((OBJECT *)(&stack0x000005f8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fb3;
      FUN_140001828((OBJECT *)(&stack0x000009b8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fc0;
      FUN_140001828((OBJECT *)(&stack0x00000d18 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fcd;
      FUN_140001828((OBJECT *)(&stack0x00000bf8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fda;
      FUN_140001828((OBJECT *)(&stack0x00000778 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001fe7;
      FUN_140001828((OBJECT *)(&stack0x00000c58 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140001ff4;
      FUN_140001828((OBJECT *)(&stack0x00000d78 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002000;
      ARRAY::~ARRAY((ARRAY *)(&stack0x00000058 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002014;
      ARRAY::~ARRAY((ARRAY *)(&stack0x00000110 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002028;
      ARGUMENT_LEXEMIZER::~ARGUMENT_LEXEMIZER((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000203c;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000003f8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002050;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000002a8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002064;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000002d8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002078;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000308 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000208c;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000218 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400020a0;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000178 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400020b4;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000338 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400020c8;
      DSTRING::~DSTRING((DSTRING *)(&stack0x000000b8 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400020dc;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000248 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400020f0;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000428 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002104;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000088 + lVar12));
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002118;
      DSTRING::~DSTRING((DSTRING *)(&stack0x00000468 + lVar12));
      uVar4 = 3;
      goto LAB_140004797;
    }
    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002142;
    uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000003f8 + lVar12),":",0xffffffff);
    if (uVar2 == '\0') {
LAB_140004425:
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004433;
      _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
      *(undefined4 *)(&stack0x00000018 + lVar12) = 0xec;
    }
    else {
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002168;
      uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000138 + lVar12),"NTFS",0xffffffff);
      if (uVar2 == '\0') goto LAB_140004425;
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000218e;
      uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000278 + lVar12),"UDF",0xffffffff);
      if (uVar2 == '\0') goto LAB_140004425;
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400021b4;
      uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000003c8 + lVar12),"REFS",0xffffffff);
      if (uVar2 == '\0') goto LAB_140004425;
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400021da;
      uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000398 + lVar12),"EXFAT",0xffffffff);
      if (uVar2 == '\0') goto LAB_140004425;
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002200;
      uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000368 + lVar12),"FAT",0xffffffff);
      if (uVar2 == '\0') goto LAB_140004425;
      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002223;
      uVar2 = ARRAY::Initialize((ARRAY *)(&stack0x00000058 + lVar12),5,1);
      if (uVar2 == '\0') {
LAB_140004405:
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004413;
        _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
        *(undefined4 *)(&stack0x00000018 + lVar12) = 0x113;
      }
      else {
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002244;
        uVar2 = ARRAY::Initialize((ARRAY *)(&stack0x00000110 + lVar12),5,1);
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002268;
        uVar2 = ARGUMENT_LEXEMIZER::Initialize
                          ((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12),
                           (ARRAY *)(&stack0x00000110 + lVar12));
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000228b;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000d78 + lVar12),"/?");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400022ae;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000cb8 + lVar12),"/O");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400022d1;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000c58 + lVar12),"/X");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400022f4;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000778 + lVar12),"/F");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002317;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000d18 + lVar12),"/V");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000233a;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x000009b8 + lVar12),"/R");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000235d;
        uVar2 = LONG_ARGUMENT::Initialize((LONG_ARGUMENT *)(&stack0x000005f8 + lVar12),"/I:*");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002380;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000718 + lVar12),"/I");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400023a3;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000958 + lVar12),"/C");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400023c6;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x000006b8 + lVar12),"/B");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400023e9;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000b98 + lVar12),"/L");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000240c;
        uVar2 = LONG_ARGUMENT::Initialize((LONG_ARGUMENT *)(&stack0x000008f8 + lVar12),"/L:*");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000242f;
        uVar2 = STRING_ARGUMENT::Initialize((STRING_ARGUMENT *)(&stack0x00000dd8 + lVar12),"*");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002455;
        uVar2 = PATH_ARGUMENT::Initialize((PATH_ARGUMENT *)(&stack0x00000e68 + lVar12),"*",'\0');
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002478;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000598 + lVar12),"/SCAN");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000249b;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000658 + lVar12),"/SPOTFIX");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400024be;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000838 + lVar12),"/VERIFY");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400024e1;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000898 + lVar12),"/SDCLEANUP")
        ;
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002504;
        uVar2 = FLAG_ARGUMENT::Initialize
                          ((FLAG_ARGUMENT *)(&stack0x00000a18 + lVar12),"/OFFLINESCANANDFIX");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002527;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000b38 + lVar12),"/PERF");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000254a;
        uVar2 = FLAG_ARGUMENT::Initialize
                          ((FLAG_ARGUMENT *)(&stack0x00000bf8 + lVar12),"/FORCEOFFLINEFIX");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000256d;
        uVar2 = FLAG_ARGUMENT::Initialize
                          ((FLAG_ARGUMENT *)(&stack0x00000ad8 + lVar12),"/FREEORPHANEDCHAINS");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002590;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x000007d8 + lVar12),"/MARKCLEAN")
        ;
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400025b3;
        uVar2 = FLAG_ARGUMENT::Initialize((FLAG_ARGUMENT *)(&stack0x00000a78 + lVar12),"/NOLOGS");
        if (uVar2 == '\0') goto LAB_140004405;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400025d1;
        ARGUMENT_LEXEMIZER::SetCaseSensitive((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12),'\0')
        ;
        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400025ea;
        uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                           (OBJECT *)(&stack0x00000dd8 + lVar12));
        if (uVar2 == '\0') {
LAB_1400043e5:
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400043f3;
          _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
          *(undefined4 *)(&stack0x00000018 + lVar12) = 0x137;
        }
        else {
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000260b;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000d78 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000262c;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000cb8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000264d;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000c58 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000266e;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000778 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000268f;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000d18 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400026b0;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x000009b8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400026d1;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x000005f8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400026f2;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000718 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002713;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000958 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002734;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x000006b8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002755;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000b98 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002776;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x000008f8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002797;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000e68 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400027b8;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000598 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400027d9;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000658 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400027fa;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000838 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000281b;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000898 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000283c;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000a18 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000285d;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000b38 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000287e;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000bf8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000289f;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000ad8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400028c0;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x000007d8 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400028e1;
          uVar2 = ARRAY::Put((ARRAY *)(&stack0x00000058 + lVar12),
                             (OBJECT *)(&stack0x00000a78 + lVar12));
          if (uVar2 == '\0') goto LAB_1400043e5;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400028ff;
          uVar2 = ARGUMENT_LEXEMIZER::PrepareToParse
                            ((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12),(WSTRING *)0x0);
          if (uVar2 == '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002916;
            _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
            *(undefined4 *)(&stack0x00000018 + lVar12) = 0x142;
            goto LAB_140002928;
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002971;
          uVar2 = ARGUMENT_LEXEMIZER::DoParsing
                            ((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12),
                             (ARRAY *)(&stack0x00000058 + lVar12));
          if (uVar2 == '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000299d;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0xbd2,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400029b1;
            pWVar16 = ARGUMENT_LEXEMIZER::QueryInvalidArgument
                                ((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12));
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400029d2;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"%W",pWVar16);
            uVar4 = 3;
            if (pWVar16 != (WSTRING *)0x0) {
              pcVar1 = (code *)**(undefined8 **)pWVar16;
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400029fb;
              (*pcVar1)(pWVar16,1);
            }
            goto LAB_14000444a;
          }
          if ((&stack0x00000dcc)[lVar12] != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002a2d;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x428,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002a48;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002a67;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x419,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002a82;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002aa1;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x429,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002abc;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002adb;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x419,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002af6;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002b15;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x42a,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002b30;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002b4f;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x42b,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002b6a;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002b89;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x42c,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002ba4;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            uVar20 = 0x42d;
LAB_140002bae:
            this = (CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12);
LAB_1400032c9:
            uVar22 = 3;
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400032d0;
            CHKDSK_MESSAGE::Set(this,uVar20,0,3);
LAB_1400032d5:
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400032eb;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
            uVar4 = uVar22;
            goto LAB_14000444a;
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002bd0;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000778 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002be8;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000007d8 + lVar12));
            if (uVar2 != '\0') {
              uVar20 = 0x792f;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c0c;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000b98 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c24;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000778 + lVar12));
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c3c;
              uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000a18 + lVar12));
              if (uVar2 == '\0') goto LAB_140002c54;
            }
            uVar20 = 0x6667;
            goto LAB_140002bae;
          }
LAB_140002c54:
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c63;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000e68 + lVar12));
          if (uVar2 == '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c7b;
            uVar2 = SYSTEM::QueryCurrentDosDriveName((WSTRING *)(&stack0x000000b8 + lVar12));
            if (uVar2 != '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002c9e;
              uVar2 = PATH::Initialize((PATH *)(&stack0x00001558 + lVar12),
                                       (WSTRING *)(&stack0x000000b8 + lVar12),'\0');
              if (uVar2 != '\0') {
                this_00 = (PATH *)(&stack0x00001558 + lVar12);
                goto LAB_140002cd9;
              }
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002cbf;
            _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
            *(undefined4 *)(&stack0x00000018 + lVar12) = 0x186;
            goto LAB_140002928;
          }
          this_00 = *(PATH **)(&stack0x00000eb8 + lVar12);
LAB_140002cd9:
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002ce8;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000598 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d00;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000658 + lVar12));
            if (uVar2 != '\0') {
              uVar20 = 0x6670;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d27;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000838 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d3f;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000778 + lVar12));
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d57;
              uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000a18 + lVar12));
              if (uVar2 == '\0') goto LAB_140002d6f;
            }
            uVar20 = 0x666d;
            goto LAB_140002bae;
          }
LAB_140002d6f:
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d7e;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000598 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002d96;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000838 + lVar12));
            if (uVar2 != '\0') {
              uVar20 = 0x666e;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002dbd;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000838 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002dd5;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000658 + lVar12));
            if (uVar2 != '\0') {
              uVar20 = 0x666f;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002dfc;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000b38 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e14;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000598 + lVar12));
            if (uVar2 == '\0') {
              uVar20 = 0x43e;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e3b;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000898 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e53;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000009b8 + lVar12));
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e6b;
              uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000006b8 + lVar12));
              if (uVar2 == '\0') {
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e83;
                uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000718 + lVar12));
                if (uVar2 == '\0') {
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002e9b;
                  uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000958 + lVar12));
                  if (uVar2 == '\0') goto LAB_140002eb3;
                }
              }
            }
            uVar20 = 0x43d;
            goto LAB_140002bae;
          }
LAB_140002eb3:
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002ec2;
          uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000bf8 + lVar12));
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002eda;
            uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000598 + lVar12));
            if (uVar2 == '\0') {
              uVar20 = 0x43f;
              goto LAB_140002bae;
            }
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002f14;
          PVar5 = PATH::AnalyzePath(this_00,(WSTRING *)(&stack0x000000b8 + lVar12),
                                    (PATH *)(&stack0x000012f8 + lVar12),
                                    (WSTRING *)(&stack0x000001b8 + lVar12));
          if ((PVar5 != 0) && (PVar5 != 1)) {
            if (PVar5 == 2) {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002f4d;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 0x21b;
              goto LAB_140002928;
            }
            if (PVar5 == 4) {
              uVar20 = 0x439;
              goto LAB_140002bae;
            }
LAB_140003363:
            uVar20 = 0x454;
            goto LAB_140002bae;
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002f6e;
          uVar2 = PATH::IsDrive((PATH *)(&stack0x000012f8 + lVar12));
          cVar21 = '\x01';
          if ((uVar2 != '\0') ||
             ((&stack0x00000040)[lVar12] = 0, *(int *)(&stack0x00001528 + lVar12) == 0)) {
            (&stack0x00000040)[lVar12] = 1;
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002f94;
          uVar2 = PATH::IsGuidVolName(this_00);
          if (uVar2 != '\0') {
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002fba;
            uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000001e8 + lVar12),
                                        (WSTRING *)(&stack0x000000b8 + lVar12),0,0xffffffff);
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002fd1;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 500;
              goto LAB_140002928;
            }
LAB_14000301b:
            if ((*(int *)(&stack0x00001528 + lVar12) == 2) &&
               (*(short *)(*(longlong *)(&stack0x00001520 + lVar12) + 2) == 0x3a)) {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003051;
              uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000000b8 + lVar12),
                                          (WSTRING *)(&stack0x00001510 + lVar12),0,0xffffffff);
              if (uVar2 == '\0') {
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003068;
                _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
                *(undefined4 *)(&stack0x00000018 + lVar12) = 0x20b;
                goto LAB_140002928;
              }
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003091;
            uVar2 = PATH::AppendString((PATH *)(&stack0x000012f8 + lVar12),
                                       (WSTRING *)(&stack0x000001b8 + lVar12));
            if (uVar2 == '\0') {
LAB_1400043c5:
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400043d3;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 0x213;
              goto LAB_140002928;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400030b8;
            uVar2 = PATH::Initialize((PATH *)(&stack0x00001558 + lVar12),
                                     (WSTRING *)(&stack0x000001b8 + lVar12),'\0');
            if (uVar2 == '\0') goto LAB_1400043c5;
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400030d4;
            pWVar16 = WSTRING::Strupr((WSTRING *)(&stack0x000000b8 + lVar12));
            if (pWVar16 == (WSTRING *)0x0) {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400030ec;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 0x22c;
              goto LAB_140002928;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003107;
            UVar6 = SetErrorMode(1);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000311e;
            DVar7 = SYSTEM::QueryDriveType((WSTRING *)(&stack0x000000b8 + lVar12));
            *(DRIVE_TYPE *)(&stack0x00000588 + lVar12) = DVar7;
            uVar4 = 3;
            if (DVar7 == 3) {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000313d;
              SetErrorMode(UVar6);
              uVar20 = 0x3f5;
              goto LAB_140002bae;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003153;
            SetErrorMode(UVar6);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003167;
            uVar2 = SYSTEM::QueryCurrentDosDriveName((WSTRING *)(&stack0x00000338 + lVar12));
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000317e;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 0x248;
              goto LAB_140002928;
            }
            if (((((&stack0x000007cc)[lVar12] != '\0') || ((&stack0x00000cac)[lVar12] != '\0')) ||
                (((&stack0x00000a0c)[lVar12] != '\0' && ((&stack0x000005ec)[lVar12] == '\0')))) ||
               (((((&stack0x0000070c)[lVar12] != '\0' || ((&stack0x000006ac)[lVar12] != '\0')) ||
                 ((&stack0x000008ec)[lVar12] != '\0')) ||
                (((&stack0x00000a6c)[lVar12] != '\0' ||
                 (cVar26 = '\0', (&stack0x0000082c)[lVar12] != '\0')))))) {
              cVar26 = cVar21;
            }
            (&stack0x00000042)[lVar12] = cVar26;
            (&stack0x00000045)[lVar12] = cVar26;
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003211;
            uVar2 = IFS_SYSTEM::DosDriveNameToNtDriveName
                              ((WSTRING *)(&stack0x000000b8 + lVar12),
                               (WSTRING *)(&stack0x00000178 + lVar12));
            if (uVar2 == '\0') {
LAB_1400043a5:
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400043b3;
              _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
              *(undefined4 *)(&stack0x00000018 + lVar12) = 0x264;
              goto LAB_140002928;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003238;
            uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000218 + lVar12),
                                        *(ushort **)(&stack0x00000188 + lVar12),0xffffffff);
            if (uVar2 == '\0') goto LAB_1400043a5;
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000324e;
            UVar6 = SetErrorMode(1);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000327d;
            uVar2 = IFS_SYSTEM::QueryFileSystemName
                              ((WSTRING *)(&stack0x00000178 + lVar12),
                               (WSTRING *)(&stack0x00000088 + lVar12),
                               (long *)(&stack0x00000168 + lVar12),
                               (WSTRING *)(&stack0x00000428 + lVar12));
            uVar22 = 0;
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003294;
              SetErrorMode(UVar6);
              this = (CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12);
              if (*(int *)(&stack0x00000168 + lVar12) == -0x3fffffde) {
                uVar20 = 0xbd5;
              }
              else {
                if (*(int *)(&stack0x00000168 + lVar12) == 0) {
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003301;
                  CHKDSK_MESSAGE::Set(this,0xbcb,0,3);
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003324;
                  MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"%W",
                                   &stack0x000001e8 + lVar12);
                  goto LAB_14000444a;
                }
                uVar20 = 0x7d7;
              }
              goto LAB_1400032c9;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003335;
            SetErrorMode(UVar6);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003351;
            uVar2 = WSTRING::operator==((WSTRING *)(&stack0x00000088 + lVar12),
                                        (WSTRING *)(&stack0x00000138 + lVar12));
            if ((uVar2 != '\0') && (*(int *)(&stack0x000001d0 + lVar12) != 0)) goto LAB_140003363;
            (&stack0x000004d8)[lVar12] = (&stack0x00000acc)[lVar12] == '\0';
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003397;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x430,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400033ba;
            MESSAGE::Log((MESSAGE *)(&stack0x00000498 + lVar12),"%W");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400033d9;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0xbc7,0,3);
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400033fc;
            MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"%W");
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003418;
            uVar2 = WSTRING::operator==((WSTRING *)(&stack0x00000088 + lVar12),
                                        (WSTRING *)(&stack0x000003c8 + lVar12));
            if (uVar2 == '\0') {
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003456;
              pWVar16 = WSTRING::Strupr((WSTRING *)(&stack0x00000088 + lVar12));
              if (pWVar16 == (WSTRING *)0x0) {
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000346e;
                _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
                *(undefined4 *)(&stack0x00000018 + lVar12) = 0x2b4;
                goto LAB_140002928;
              }
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000348f;
              DSTRING::DSTRING((DSTRING *)(&stack0x00000e38 + lVar12));
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400034ad;
              uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000e38 + lVar12),"FAT32",0xffffffff)
              ;
              if (uVar2 == '\0') {
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400034c4;
                _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
                *(undefined4 *)(&stack0x00000018 + lVar12) = 700;
LAB_140004370:
                *(__uint64 *)(&stack0x00000010 + lVar12) = _Var15;
                *(char **)(&stack0x00000008 + lVar12) = "%I64x%x";
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000439b;
                MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x43c,1,3,
                                    *(char **)(&stack0x00000008 + lVar12));
              }
              else {
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400034ed;
                uVar2 = WSTRING::operator==((WSTRING *)(&stack0x00000088 + lVar12),
                                            (WSTRING *)(&stack0x00000e38 + lVar12));
                if (uVar2 != '\0') {
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000350f;
                  WSTRING::Initialize((WSTRING *)(&stack0x00000088 + lVar12),"FAT",0xffffffff);
                }
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000352d;
                uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000248 + lVar12),"U",0xffffffff);
                if (uVar2 == '\0') {
LAB_140004355:
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004363;
                  _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
                  *(undefined4 *)(&stack0x00000018 + lVar12) = 0x2cc;
                  goto LAB_140004370;
                }
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003551;
                uVar2 = WSTRING::Strcat((WSTRING *)(&stack0x00000248 + lVar12),
                                        (WSTRING *)(&stack0x00000088 + lVar12));
                if (uVar2 == '\0') goto LAB_140004355;
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003577;
                uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000308 + lVar12),"ChkdskEx",
                                            0xffffffff);
                if (uVar2 == '\0') goto LAB_140004355;
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000359d;
                uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000002d8 + lVar12),
                                            "SetOriginalVolumeName",0xffffffff);
                if (uVar2 == '\0') goto LAB_140004355;
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400035c3;
                uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000002a8 + lVar12),
                                            "SetWriteViewCacheVolumeName",0xffffffff);
                if (uVar2 == '\0') goto LAB_140004355;
                if (cVar26 != '\0') {
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400035ec;
                  uVar2 = WSTRING::operator==((WSTRING *)(&stack0x00000338 + lVar12),
                                              (WSTRING *)(&stack0x000000b8 + lVar12));
                  if (uVar2 != '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000360f;
                    CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0xbd6,0,3);
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000362a;
                    MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
                  }
                }
                *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000363e;
                uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000005f8 + lVar12));
                if ((uVar2 == '\0') || ((&stack0x0000076c)[lVar12] == '\0')) {
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000369f;
                  uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000005f8 + lVar12));
                  if (uVar2 == '\0') {
                    uVar8 = 0;
                    *(undefined4 *)(&stack0x00000084 + lVar12) = 0;
                  }
                  else {
                    uVar8 = *(uint *)(&stack0x00000648 + lVar12);
                    *(uint *)(&stack0x00000084 + lVar12) = uVar8;
                    if (0x32 < uVar8) {
                      uVar20 = 0x46b;
                      goto LAB_140003656;
                    }
                  }
                  *(short *)(&stack0x00000048 + lVar12) = (short)uVar8;
                  if ((&stack0x0000076c)[lVar12] == '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400036ea;
                    uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000005f8 + lVar12));
                    if (uVar2 != '\0') goto LAB_1400036f3;
                  }
                  else {
LAB_1400036f3:
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000370a;
                    lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                             (WSTRING *)(&stack0x00000138 + lVar12));
                    if (lVar9 != 0) {
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000372a;
                      lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                               (WSTRING *)(&stack0x00000278 + lVar12));
                      if (lVar9 != 0) {
                        uVar20 = 0x45f;
                        goto LAB_140003656;
                      }
                    }
                  }
                  if ((&stack0x000009ac)[lVar12] != '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000375e;
                    lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                             (WSTRING *)(&stack0x00000138 + lVar12));
                    if (lVar9 != 0) {
                      uVar20 = 0x460;
                      goto LAB_140003656;
                    }
                  }
                  if (((&stack0x00000b2c)[lVar12] != '\0') || ((&stack0x0000082c)[lVar12] != '\0'))
                  {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000379c;
                    lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                             (WSTRING *)(&stack0x00000368 + lVar12));
                    if (lVar9 != 0) {
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400037bc;
                      lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                               (WSTRING *)(&stack0x00000e38 + lVar12));
                      if (lVar9 != 0) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400037dc;
                        lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                                 (WSTRING *)(&stack0x00000398 + lVar12));
                        if (lVar9 != 0) {
                          uVar20 = 0x792e;
                          goto LAB_140003656;
                        }
                      }
                    }
                  }
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400037fe;
                  uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x00000b98 + lVar12));
                  if (uVar2 == '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003816;
                    uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000008f8 + lVar12));
                    cVar25 = '\0';
                    if (uVar2 != '\0') goto LAB_140003822;
                  }
                  else {
LAB_140003822:
                    cVar25 = cVar21;
                  }
                  (&stack0x00000043)[lVar12] = cVar25;
                  (&stack0x00000044)[lVar12] = cVar25;
                  puVar24 = (undefined1 *)0x0;
                  if (cVar25 != '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000384b;
                    lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                             (WSTRING *)(&stack0x00000138 + lVar12));
                    if (lVar9 == 0) {
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000386d;
                      uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000008f8 + lVar12));
                      if (uVar2 == '\0') {
                        *(undefined4 *)(&stack0x0000004c + lVar12) = 0;
                        puVar24 = (undefined1 *)0x0;
                        goto LAB_1400038af;
                      }
                      uVar8 = *(uint *)(&stack0x00000948 + lVar12);
                      if ((int)uVar8 < 1) {
                        uVar20 = 0x452;
                      }
                      else {
                        if (uVar8 < 0x400000) {
                          *(uint *)(&stack0x0000004c + lVar12) = uVar8 << 10;
                          puVar24 = (undefined1 *)(ulonglong)(uVar8 << 10);
                          goto LAB_1400038af;
                        }
                        uVar20 = 0x6608;
                      }
                    }
                    else {
                      uVar20 = 0x453;
                    }
                    goto LAB_140003656;
                  }
LAB_1400038af:
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400038be;
                  uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000006b8 + lVar12));
                  if (uVar2 != '\0') {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400038de;
                    lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                             (WSTRING *)(&stack0x00000138 + lVar12));
                    if (lVar9 != 0) {
                      uVar20 = 0x46f;
                      goto LAB_140003656;
                    }
                  }
                  while( true ) {
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003910;
                    p_Var17 = SYSTEM::QueryLibraryEntryPoint
                                        ((WSTRING *)(&stack0x00000248 + lVar12),
                                         (WSTRING *)(&stack0x00000308 + lVar12),
                                         (void **)(&stack0x00000458 + lVar12));
                    *(_func___int64 **)(&stack0x000001b0 + lVar12) = p_Var17;
                    if (p_Var17 == (_func___int64 *)0x0) break;
                    *(undefined2 *)(&stack0x00000f08 + lVar12) = 0x101;
                    *(uint *)(&stack0x00000f0c + lVar12) =
                         (uint)(byte)(&stack0x00000044)[lVar12] << 6 |
                         -(uint)((&stack0x00000cac)[lVar12] != '\0') & 0x100 |
                         (uint)((&stack0x00000d6c)[lVar12] != '\0') |
                         -(uint)((&stack0x00000a0c)[lVar12] != '\0') & 6 |
                         -(uint)((&stack0x0000076c)[lVar12] != '\0') & 0x200 |
                         -(uint)((&stack0x000009ac)[lVar12] != '\0') & 0x400 |
                         -(uint)((&stack0x0000070c)[lVar12] != '\0') & 0x2000;
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400039c1;
                    uVar2 = ARGUMENT::IsValueSet((ARGUMENT *)(&stack0x000005f8 + lVar12));
                    if (((&stack0x000006ac)[lVar12] != '\0') ||
                       (uVar8 = uVar22, (&stack0x0000088c)[lVar12] != '\0')) {
                      uVar8 = 0x4000;
                    }
                    *(uint *)(&stack0x00000f0c + lVar12) =
                         -(uint)((&stack0x000005ec)[lVar12] != '\0') & 0x8000 |
                         *(uint *)(&stack0x00000f0c + lVar12) | -(uint)(uVar2 != '\0') & 0x800 |
                         uVar8 | -(uint)((&stack0x000008ec)[lVar12] != '\0') & 0x10000 |
                         -(uint)((&stack0x00000b8c)[lVar12] != '\0') & 0x20000 |
                         -(uint)((&stack0x00000c4c)[lVar12] != '\0') & 0x40000 |
                         -(uint)((&stack0x00000b2c)[lVar12] != '\0') & 0x80000 |
                         -(uint)((&stack0x0000082c)[lVar12] != '\0') & 0x100000 |
                         -(uint)((&stack0x00000acc)[lVar12] != '\0') & 0x200000;
                    *(int *)(&stack0x00000f10 + lVar12) = (int)puVar24;
                    *(undefined1 **)(&stack0x00000f18 + lVar12) = &stack0x000012f8 + lVar12;
                    puVar18 = &stack0x00001558 + lVar12;
                    if ((&stack0x00000040)[lVar12] != '\0') {
                      puVar18 = (undefined1 *)0x0;
                    }
                    *(undefined1 **)(&stack0x00000f20 + lVar12) = puVar18;
                    *(short *)(&stack0x00000f28 + lVar12) =
                         (short)*(undefined4 *)(&stack0x00000084 + lVar12);
                    if (cVar26 == '\0') {
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003b2b;
                      p_Var17 = SYSTEM::QueryNextLibraryEntryPoint
                                          (*(void **)(&stack0x00000458 + lVar12),
                                           (WSTRING *)(&stack0x000002d8 + lVar12));
                      *(_func___int64 **)(&stack0x000012e8 + lVar12) = p_Var17;
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003b4f;
                      p_Var17 = SYSTEM::QueryNextLibraryEntryPoint
                                          (*(void **)(&stack0x00000458 + lVar12),
                                           (WSTRING *)(&stack0x000002a8 + lVar12));
                      *(_func___int64 **)(&stack0x00000ec8 + lVar12) = p_Var17;
                      if (((*(uint *)(&stack0x00000f0c + lVar12) & 0x4000) != 0) ||
                         ((&stack0x00000041)[lVar12] = 0,
                         (*(uint *)(&stack0x00000f0c + lVar12) & 0x8000) != 0)) {
                        (&stack0x00000041)[lVar12] = 1;
                      }
                      cVar23 = '\0';
                      (&stack0x00000038)[lVar12] = 0;
                      if ((((&stack0x00000d0c)[lVar12] == '\0') && (cVar25 == '\0')) &&
                         (*(int *)(&stack0x00000588 + lVar12) != 4)) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003bc4;
                        lVar9 = SNAPSHOT::GetVolumeSnapshot
                                          ((WSTRING *)(&stack0x000000b8 + lVar12),
                                           (SNAPSHOT **)(&stack0x000001a8 + lVar12));
                        *(long *)(&stack0x00000080 + lVar12) = lVar9;
                        if ((lVar9 < 0) ||
                           (*(SNAPSHOT **)(&stack0x000001a8 + lVar12) == (SNAPSHOT *)0x0)) {
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003ca7;
                          DSTRING::DSTRING((DSTRING *)(&stack0x00000ed8 + lVar12));
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003cc2;
                          uVar2 = SNAPSHOT::GetSnapshotErrorMessage
                                            (*(long *)(&stack0x00000080 + lVar12),
                                             (WSTRING *)(&stack0x00000ed8 + lVar12));
                          if (uVar2 != '\0') {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003cee;
                            MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x41e,"%W");
                          }
                        }
                        else {
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003bf0;
                          puVar19 = SNAPSHOT::GetSnapshotNtDeviceName
                                              (*(SNAPSHOT **)(&stack0x000001a8 + lVar12));
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003c0a;
                          uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x00000178 + lVar12),puVar19
                                                      ,0xffffffff);
                          if (uVar2 != '\0') {
                            (&stack0x00000038)[lVar12] = 1;
                            *(uint *)(&stack0x00000f0c + lVar12) =
                                 *(uint *)(&stack0x00000f0c + lVar12) | 0x1000;
                            cVar23 = cVar21;
                            goto LAB_140003d07;
                          }
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003c38;
                          DSTRING::DSTRING((DSTRING *)(&stack0x00000ed8 + lVar12));
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003c51;
                          uVar2 = SNAPSHOT::GetSnapshotErrorMessage
                                            (-0x7ff8fa56,(WSTRING *)(&stack0x00000ed8 + lVar12));
                          if (uVar2 != '\0') {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003c7d;
                            MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x41e,"%W");
                          }
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003c91;
                          SNAPSHOT::ReleaseVolumeSnapshot(*(SNAPSHOT **)(&stack0x000001a8 + lVar12))
                          ;
                        }
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003d02;
                        DSTRING::~DSTRING((DSTRING *)(&stack0x00000ed8 + lVar12));
                      }
LAB_140003d07:
                      if (((&stack0x00000041)[lVar12] == '\0') || (cVar23 != '\0')) {
                        if (*(code **)(&stack0x000012e8 + lVar12) != (code *)0x0) {
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003d5d;
                          (**(code **)(&stack0x000012e8 + lVar12))(&stack0x00000218 + lVar12);
                        }
                        if ((cVar23 != '\0') && (*(longlong *)(&stack0x00000ec8 + lVar12) != 0)) {
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003d7b;
                          DSTRING::DSTRING((DSTRING *)(&stack0x00000ed8 + lVar12));
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003d97;
                          uVar2 = SNAPSHOT::QuerySnapshotDiffAreaVolume
                                            (*(SNAPSHOT **)(&stack0x000001a8 + lVar12),
                                             (WSTRING *)(&stack0x00000ed8 + lVar12));
                          if (uVar2 != '\0') {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003dc0;
                            (**(code **)(&stack0x00000ec8 + lVar12))(&stack0x00000ed8 + lVar12);
                          }
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003dcf;
                          DSTRING::~DSTRING((DSTRING *)(&stack0x00000ed8 + lVar12));
                        }
                        *(undefined1 **)(&stack0x00000008 + lVar12) = &stack0x0000003c + lVar12;
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003e11;
                        (**(code **)(&stack0x000001b0 + lVar12))
                                  (&stack0x00000178 + lVar12,&stack0x00000498 + lVar12);
                      }
                      else {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003d21;
                        uVar2 = SNAPSHOT::IsFatalError(*(long *)(&stack0x00000080 + lVar12));
                        *(uint *)(&stack0x0000003c + lVar12) = (uVar2 != '\0') + 10;
                      }
                      if (*(int *)(&stack0x0000003c + lVar12) == 9) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003e9e;
                        MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6fc8);
                      }
                      if (*(int *)(&stack0x0000003c + lVar12) == 10) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003ebe;
                        MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6675);
                      }
                      if (*(int *)(&stack0x0000003c + lVar12) == 0xb) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003ede;
                        MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6676);
                      }
                      if (cVar23 != '\0') {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003ef7;
                        SNAPSHOT::ReleaseVolumeSnapshot(*(SNAPSHOT **)(&stack0x000001a8 + lVar12));
                      }
                      puVar24 = (undefined1 *)(ulonglong)*(uint *)(&stack0x0000004c + lVar12);
                    }
                    else {
                      *(undefined1 **)(&stack0x00000008 + lVar12) = &stack0x0000003c + lVar12;
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003b0f;
                      (**(code **)(&stack0x000001b0 + lVar12))
                                (&stack0x00000178 + lVar12,&stack0x00000498 + lVar12);
                    }
                    if (*(int *)(&stack0x0000003c + lVar12) == 1) {
                      uVar10 = 1;
                      if (cVar26 == '\0') {
                        uVar10 = 3;
                      }
                      *(undefined4 *)(&stack0x0000003c + lVar12) = uVar10;
                    }
                    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003f22;
                    SYSTEM::FreeLibraryHandle(*(void **)(&stack0x00000458 + lVar12));
                    *(undefined8 *)(&stack0x00000458 + lVar12) = 0;
                    if ((*(uint *)(&stack0x00000f0c + lVar12) & 0x8000) == 0) {
LAB_140003f7e:
                      if (*(int *)(&stack0x0000003c + lVar12) - 10U < 2) {
                        *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003fa4;
                        lVar9 = WSTRING::Stricmp((WSTRING *)(&stack0x00000088 + lVar12),
                                                 (WSTRING *)(&stack0x00000138 + lVar12));
                        if (lVar9 == 0) {
                          (&stack0x00000038)[lVar12] = 0;
                          *(undefined8 *)(&stack0x000001b0 + lVar12) = 0;
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003fcd;
                          DP_DRIVE::DP_DRIVE((DP_DRIVE *)(&stack0x00001168 + lVar12));
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003fec;
                          IFS_SYSTEM::IsVolumeDirty
                                    ((WSTRING *)(&stack0x00000218 + lVar12),
                                     &stack0x00000038 + lVar12,(uchar *)0x0,(long *)0x0);
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004008;
                          IFS_SYSTEM::QueryVolumeSize
                                    ((WSTRING *)(&stack0x000000b8 + lVar12),
                                     (__uint64 *)(&stack0x000001b0 + lVar12));
                          (&stack0x00000008)[lVar12] = 0;
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000402f;
                          uVar2 = DP_DRIVE::Initialize
                                            ((DP_DRIVE *)(&stack0x00001168 + lVar12),
                                             (WSTRING *)(&stack0x00000218 + lVar12),(MESSAGE *)0x0,
                                             '\0',(&stack0x00000008)[lVar12]);
                          if (uVar2 == '\0') {
LAB_14000405b:
                            *(undefined8 *)(&stack0x000012e8 + lVar12) = 0xfefefefefefefefe;
                            *(undefined8 *)(&stack0x000012f0 + lVar12) = 0xfefefefefefefefe;
                          }
                          else {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004052;
                            uVar2 = DP_DRIVE::QueryID((DP_DRIVE *)(&stack0x00001168 + lVar12),
                                                      (_GUID *)(&stack0x000012e8 + lVar12),
                                                      (WSTRING *)0x0);
                            if (uVar2 == '\0') goto LAB_14000405b;
                          }
                          *(undefined8 *)(&stack0x00000ec8 + lVar12) =
                               *(undefined8 *)(&stack0x000012e8 + lVar12);
                          *(undefined8 *)(&stack0x00000ed0 + lVar12) =
                               *(undefined8 *)(&stack0x000012f0 + lVar12);
                          *(undefined1 **)(&stack0x00000010 + lVar12) = &stack0x00000ec8 + lVar12;
                          *(undefined8 *)(&stack0x00000008 + lVar12) =
                               *(undefined8 *)(&stack0x000001b0 + lVar12);
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400040b0;
                          SQMEXPORT::SqmExportOnError
                                    (*(undefined4 *)(&stack0x0000003c + lVar12),
                                     *(undefined4 *)(&stack0x00000f0c + lVar12),cVar26,
                                     (&stack0x00000038)[lVar12]);
                          uVar4 = *(uint *)(&stack0x00000f0c + lVar12);
                          if ((uVar4 & 0x8000) == 0) {
                            if ((uVar4 >> 0xe & 1) == 0) {
                              if (cVar26 == '\0') {
                                DVar11 = ~(uVar4 >> 0xc) & 1 | 0x6664;
                              }
                              else {
                                DVar11 = 0x6666;
                              }
                            }
                            else {
                              DVar11 = ((byte)(&stack0x00000045)[lVar12] ^ 1) + 0x6673;
                            }
                          }
                          else {
                            DVar11 = 0x6672;
                          }
                          *(undefined4 *)(&stack0x00000050 + lVar12) = 0;
                          if ((&stack0x000004d8)[lVar12] == '\0') {
LAB_140004186:
                            *(undefined4 *)(&stack0x00000050 + lVar12) = 0;
                            *(undefined8 *)(&stack0x00000460 + lVar12) = 0;
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400041a6;
                            MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6645);
                          }
                          else {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004119;
                            uVar2 = HMEM::Initialize((HMEM *)(&stack0x000000e8 + lVar12));
                            if (uVar2 == '\0') goto LAB_140004186;
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000413e;
                            uVar2 = MESSAGE::QueryPackedLog
                                              ((MESSAGE *)(&stack0x00000498 + lVar12),
                                               (HMEM *)(&stack0x000000e8 + lVar12),
                                               (ulong *)(&stack0x00000050 + lVar12));
                            if (uVar2 == '\0') goto LAB_140004186;
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004160;
                            uVar2 = HMEM::Resize((HMEM *)(&stack0x000000e8 + lVar12),
                                                 *(int *)(&stack0x00000050 + lVar12) + 2,0);
                            if (uVar2 == '\0') goto LAB_140004186;
                            *(longlong *)(&stack0x00000460 + lVar12) =
                                 *(longlong *)(&stack0x00000108 + lVar12);
                            *(undefined2 *)
                             (*(longlong *)(&stack0x00000108 + lVar12) +
                             (ulonglong)(*(uint *)(&stack0x00000050 + lVar12) >> 1) * 2) = 0;
                          }
                          if (0x8000 < *(uint *)(&stack0x00000050 + lVar12)) {
                            *(undefined2 *)(*(longlong *)(&stack0x00000460 + lVar12) + 0x8000) = 0;
                          }
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400041d1;
                          hEventLog = RegisterEventSourceW((LPCWSTR)0x0,L"Chkdsk");
                          if (hEventLog == (HANDLE)0x0) {
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000426f;
                            MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6646);
                          }
                          else {
                            *(undefined8 *)(&stack0x00000028 + lVar12) = 0;
                            *(undefined1 **)(&stack0x00000020 + lVar12) = &stack0x00000460 + lVar12;
                            *(undefined4 *)(&stack0x00000018 + lVar12) = 0;
                            *(undefined2 *)(&stack0x00000010 + lVar12) = 1;
                            *(undefined8 *)(&stack0x00000008 + lVar12) = 0;
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004212;
                            BVar3 = ReportEventW(hEventLog,4,0,DVar11,
                                                 *(PSID *)(&stack0x00000008 + lVar12),
                                                 *(WORD *)(&stack0x00000010 + lVar12),
                                                 *(DWORD *)(&stack0x00000018 + lVar12),
                                                 *(LPCWSTR **)(&stack0x00000020 + lVar12),
                                                 *(LPVOID *)(&stack0x00000028 + lVar12));
                            if (BVar3 == 0) {
                              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004222;
                              DVar11 = GetLastError();
                              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004245;
                              MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x6647,"%d"
                                                  ,(ulonglong)DVar11);
                            }
                            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004254;
                            DeregisterEventSource(hEventLog);
                          }
                          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004283;
                          DP_DRIVE::~DP_DRIVE((DP_DRIVE *)(&stack0x00001168 + lVar12));
                        }
                      }
                      *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004297;
                      ARRAY::DeleteAllMembers((ARRAY *)(&stack0x00000110 + lVar12));
                      uVar4 = *(uint *)(&stack0x0000003c + lVar12);
                      goto LAB_1400042a0;
                    }
                    if ((*(int *)(&stack0x0000003c + lVar12) == 8) &&
                       (*(int *)(&stack0x0000016c + lVar12) != 0)) {
                      *(int *)(&stack0x0000016c + lVar12) = *(int *)(&stack0x0000016c + lVar12) + -1
                      ;
                    }
                    else {
                      if ((*(int *)(&stack0x0000003c + lVar12) != 0xc) ||
                         (*(int *)(&stack0x00000170 + lVar12) == 0)) goto LAB_140003f7e;
                      *(int *)(&stack0x00000170 + lVar12) = *(int *)(&stack0x00000170 + lVar12) + -1
                      ;
                    }
                  }
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400042d3;
                  CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0xbca,0,3);
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400042fd;
                  MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"%s%W","CHKDSK",
                                   &stack0x00000088 + lVar12);
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000431c;
                  CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x419,0,3);
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004337;
                  MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000434b;
                  ARRAY::DeleteAllMembers((ARRAY *)(&stack0x00000110 + lVar12));
                }
                else {
                  uVar20 = 0x46a;
LAB_140003656:
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000366b;
                  CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),uVar20,0,3);
                  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003686;
                  MESSAGE::Display((MESSAGE *)(&stack0x00000498 + lVar12),"");
                }
              }
LAB_1400042a0:
              *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400042af;
              DSTRING::~DSTRING((DSTRING *)(&stack0x00000e38 + lVar12));
              goto LAB_14000444a;
            }
            *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000343b;
            CHKDSK_MESSAGE::Set((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12),0x7c9c,0,3);
            goto LAB_1400032d5;
          }
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002ff2;
          uVar2 = WSTRING::Initialize((WSTRING *)(&stack0x000001e8 + lVar12),
                                      (WSTRING *)(&stack0x00001510 + lVar12),0,0xffffffff);
          if (uVar2 != '\0') goto LAB_14000301b;
          *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140003009;
          _Var15 = MESSAGE::MakeFileToken("onecore\\base\\fs\\utils\\chkdsk\\chkdsk.cxx");
          *(undefined4 *)(&stack0x00000018 + lVar12) = 0x1fc;
        }
      }
    }
LAB_140002928:
    *(__uint64 *)(&stack0x00000010 + lVar12) = _Var15;
    *(char **)(&stack0x00000008 + lVar12) = "%I64x%x";
    *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140002953;
    MESSAGE::DisplayMsg((MESSAGE *)(&stack0x00000498 + lVar12),0x43c,1,3,
                        *(char **)(&stack0x00000008 + lVar12));
    uVar4 = 3;
  }
LAB_14000444a:
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004459;
  HMEM::~HMEM((HMEM *)(&stack0x000000e8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000446d;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000368 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004481;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000398 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004495;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000003c8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400044a9;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000278 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400044bd;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000138 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400044d1;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000001b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400044e5;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000001e8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400044f9;
  PATH::~PATH((PATH *)(&stack0x00001558 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000450d;
  PATH::~PATH((PATH *)(&stack0x000012f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004521;
  CHKDSK_MESSAGE::~CHKDSK_MESSAGE((CHKDSK_MESSAGE *)(&stack0x00000498 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004533;
  FUN_140001828((OBJECT *)(&stack0x00000a78 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004540;
  FUN_140001828((OBJECT *)(&stack0x000007d8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000454d;
  FUN_140001828((OBJECT *)(&stack0x00000ad8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000455a;
  FUN_140001828((OBJECT *)(&stack0x00000a18 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004567;
  FUN_140001828((OBJECT *)(&stack0x00000b38 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004574;
  FUN_140001828((OBJECT *)(&stack0x00000898 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004581;
  FUN_140001828((OBJECT *)(&stack0x00000838 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000458e;
  FUN_140001828((OBJECT *)(&stack0x00000658 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000459b;
  FUN_140001828((OBJECT *)(&stack0x00000598 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045a8;
  FUN_140001828((OBJECT *)(&stack0x00000cb8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045b7;
  PATH_ARGUMENT::~PATH_ARGUMENT((PATH_ARGUMENT *)(&stack0x00000e68 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045cb;
  STRING_ARGUMENT::~STRING_ARGUMENT((STRING_ARGUMENT *)(&stack0x00000dd8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045dd;
  FUN_140001828((OBJECT *)(&stack0x000008f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045ea;
  FUN_140001828((OBJECT *)(&stack0x00000b98 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400045f7;
  FUN_140001828((OBJECT *)(&stack0x000006b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004604;
  FUN_140001828((OBJECT *)(&stack0x00000958 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004611;
  FUN_140001828((OBJECT *)(&stack0x00000718 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000461e;
  FUN_140001828((OBJECT *)(&stack0x000005f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000462b;
  FUN_140001828((OBJECT *)(&stack0x000009b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004638;
  FUN_140001828((OBJECT *)(&stack0x00000d18 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004645;
  FUN_140001828((OBJECT *)(&stack0x00000bf8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004652;
  FUN_140001828((OBJECT *)(&stack0x00000778 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000465f;
  FUN_140001828((OBJECT *)(&stack0x00000c58 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000466c;
  FUN_140001828((OBJECT *)(&stack0x00000d78 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004678;
  ARRAY::~ARRAY((ARRAY *)(&stack0x00000058 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000468c;
  ARRAY::~ARRAY((ARRAY *)(&stack0x00000110 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400046a0;
  ARGUMENT_LEXEMIZER::~ARGUMENT_LEXEMIZER((ARGUMENT_LEXEMIZER *)(&stack0x00000f58 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400046b4;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000003f8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400046c8;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000002a8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400046dc;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000002d8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400046f0;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000308 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004704;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000218 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004718;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000178 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000472c;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000338 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004740;
  DSTRING::~DSTRING((DSTRING *)(&stack0x000000b8 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004754;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000248 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004768;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000428 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x14000477c;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000088 + lVar12));
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x140004790;
  DSTRING::~DSTRING((DSTRING *)(&stack0x00000468 + lVar12));
LAB_140004797:
  *(undefined8 *)((longlong)&uStack_20 + lVar12) = 0x1400047a7;
  return uVar4;
}



/* ===== FUNCTION FUN_1400047d0 @ 1400047d0 ===== */

void FUN_1400047d0(_EXCEPTION_RECORD *param_1,void *param_2,_CONTEXT *param_3,
                  _DISPATCHER_CONTEXT *param_4)

{
  uint uVar1;
  uint *puVar2;
  
  puVar2 = *(uint **)(param_4 + 0x38);
  uVar1 = *puVar2;
  __GSHandlerCheckCommon(param_2,(longlong)param_4);
  if ((puVar2[(ulonglong)uVar1 * 4 + 1] & ((param_1->ExceptionFlags & 0x66) != 0) + 1) != 0) {
    __C_specific_handler(param_1,param_2,param_3,param_4);
  }
  return;
}



/* ===== FUNCTION __GSHandlerCheckCommon @ 140004868 ===== */

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* Library Function - Single Match
    __GSHandlerCheckCommon
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

ulonglong __GSHandlerCheckCommon(undefined8 param_1,longlong param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  
  uVar2 = (ulonglong)*(uint *)(*(longlong *)(param_2 + 0x10) + 8);
  uVar1 = *(ulonglong *)(param_2 + 8);
  if ((*(byte *)(uVar2 + 3 + uVar1) & 0xf) != 0) {
    uVar1 = (ulonglong)(*(byte *)(uVar2 + 3 + uVar1) & 0xfffffff0);
  }
  return uVar1;
}



/* ===== FUNCTION _guard_dispatch_icall @ 1400048e0 ===== */

/* WARNING: This is an inlined function */

void _guard_dispatch_icall(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
                    /* WARNING: Could not recover jumptable at 0x0001400048e0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



/* ===== FUNCTION _guard_dispatch_icall @ 140004900 ===== */

/* WARNING: This is an inlined function */

void _guard_dispatch_icall(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
                    /* WARNING: Could not recover jumptable at 0x0001400048e0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



/* ===== FUNCTION FUN_140004920 @ 140004920 ===== */

void FUN_140004920(void)

{
  undefined1 *in_RAX;
  undefined1 *puVar1;
  void *pvVar2;
  undefined1 local_res8 [32];
  
  puVar1 = local_res8 + -(longlong)in_RAX;
  if (local_res8 < in_RAX) {
    puVar1 = (undefined1 *)0x0;
  }
  if (puVar1 < StackLimit) {
    pvVar2 = StackLimit;
    do {
      pvVar2 = (void *)((longlong)pvVar2 - 0x1000);
    } while ((void *)((ulonglong)puVar1 & 0xfffffffffffff000) < pvVar2);
  }
  return;
}



/* ===== FUNCTION FUN_140004973 @ 140004973 ===== */

void FUN_140004973(_EXCEPTION_POINTERS *param_1)

{
  _XcptFilter(param_1->ExceptionRecord->ExceptionCode,param_1);
  return;
}



/* ===== FUNCTION FUN_1400049a0 @ 1400049a0 ===== */

bool FUN_1400049a0(undefined8 *param_1)

{
  return *(int *)*param_1 == -0x3ffffffb;
}


