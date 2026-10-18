
/* ===== FORCED FUNCTION forced_89dabf90 @ 89dabf90 ===== */

undefined4 forced_89dabf90(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;

  uVar3 = uRamb8801000 >> 0x17;
  uVar1 = uRamb8801000 & 7;
  uVar2 = uRamb8800070 >> 4 & 7;
  switch(uVar2) {
  case 0:
    uVar2 = 800;
    break;
  case 1:
    uVar2 = 0x42a;
    break;
  case 2:
    uVar2 = 0x535;
    break;
  case 3:
    uVar2 = 0x640;
    break;
  case 4:
    uVar2 = 600;
  }
  if ((short)uRamb880048a < 0) {
    if (cRamb8800003 == '\x15') {
      uVar2 = (uRamb880048a & 0x7fff) * 0xc;
    }
    else if (cRamb8800003 == '\x16') {
      uVar2 = (uRamb880048a & 0x7fff) * 0x18;
    }
  }
  func_0x89dafc90(0x89e7dd5c);
  func_0x89dafc90(0x89e7dd68,(uVar3 & 1) + 2);
  func_0x89dafc90(0x89e7dd78,0x10 << uVar1);
  func_0x89dafc90(0x89e7dd84,uVar2);
  return 0;
}
