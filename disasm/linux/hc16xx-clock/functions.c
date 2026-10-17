
/* ===== FORCED FUNCTION forced_800011ec @ 800011ec ===== */

void forced_800011ec(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iStack_18;
  int aiStack_14 [2];

  iStack_18 = -1;
  aiStack_14[0] = -1;
  iVar1 = func_0x80187b24(uRam80880970,0x8040dacc);
  if (-1 < iVar1) {
    func_0x80001180(iVar1,0x8040dad8,&iStack_18);
    func_0x80001180(iVar1,0x80432c68,aiStack_14);
  }
  iVar1 = iStack_18;
  if (iStack_18 != -1) {
    uVar2 = func_0x801b02b8(0xb8800380);
    func_0x801b02dc((iVar1 * 10 - 0x18U) / 0x18 << 0x10 | 0x80000000 | uVar2 & 0xffff,0xb8800380);
  }
  if (aiStack_14[0] != -1) {
    uVar3 = aiStack_14[0] << 8;
    uVar2 = func_0x801b02b8(0xb8800074);
    func_0x801b02dc(uVar3 | uVar2 & 0xfffff8ff,0xb8800074);
    uVar2 = func_0x801b02b8(0xb8800074);
    func_0x801b02dc(uVar2 | 0x400000,0xb8800074);
  }
  if (aiStack_14[0] == 7) {
    uVar2 = func_0x801b02b8(0xb880007c);
    func_0x801b02dc(uVar2 | 0x80,0xb880007c);
  }
  return;
}



/* ===== FORCED FUNCTION forced_802b61d8 @ 802b61d8 ===== */

undefined4 forced_802b61d8(undefined4 *param_1,undefined4 param_2)

{
  undefined4 uVar1;

  if (param_1 != (undefined4 *)0x0) {
    func_0x802b3e40();
    uVar1 = func_0x802b6120(*param_1,param_2);
    func_0x802b543c();
    return uVar1;
  }
  return 0;
}
