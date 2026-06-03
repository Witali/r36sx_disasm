
/* ===== FUNCTION entry @ 1cfb:0010 ===== */

void __cdecl16far entry(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  int unaff_ES;
  
  DAT_1cfb_0004 = unaff_ES + 0x10;
  DAT_5000_5b9e = DAT_1cfb_0004 + DAT_1cfb_000c;
  puVar4 = (undefined1 *)(DAT_1cfb_0006 + -1);
  puVar5 = puVar4;
  for (iVar3 = DAT_1cfb_0006; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar2 = puVar5;
    puVar5 = puVar5 + -1;
    puVar1 = puVar4;
    puVar4 = puVar4 + -1;
    *puVar2 = *puVar1;
  }
  DAT_5000_5b9c = 0x32;
  return;
}


