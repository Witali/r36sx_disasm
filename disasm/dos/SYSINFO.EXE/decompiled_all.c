
/* ===== FUNCTION entry @ 0ff0:0100 ===== */

/* WARNING: Instruction at (ram,0x0001002a) overlaps instruction at (ram,0x00010029)
    */
/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */
/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (ram,0x00010021) */
/* WARNING: Removing unreachable block (ram,0x00010029) */

int __cdecl16far entry(void)

{
  undefined2 *puVar1;
  uint *puVar2;
  byte *pbVar3;
  byte *pbVar4;
  code *pcVar5;
  uint uVar6;
  int iVar7;
  undefined1 uVar9;
  int iVar8;
  int in_CX;
  char cVar11;
  uint uVar10;
  int iVar12;
  byte bVar13;
  undefined2 in_BX;
  uint uVar14;
  uint uVar15;
  int *piVar16;
  uint unaff_BP;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  int unaff_SI;
  undefined2 *puVar22;
  byte *pbVar23;
  uint *puVar24;
  uint *puVar25;
  byte *unaff_DI;
  int unaff_ES;
  int unaff_DS;
  undefined2 uVar26;
  undefined2 in_GS;
  bool bVar27;
  ulong uVar28;
  
  piVar16 = (int *)0x200;
  if (*(uint *)0x2 < 0x623f) {
    pcVar5 = (code *)swi(0x21);
    (*pcVar5)();
    pcVar5 = (code *)swi(0x20);
    uVar28 = (*pcVar5)();
    uVar26 = (undefined2)(uVar28 >> 0x10);
    iVar7 = (int)uVar28;
    out(*(undefined2 *)(unaff_SI + -1),uVar26);
    if ((undefined2 *)(unaff_SI + -1) != (undefined2 *)0x0) {
      out(*(undefined1 *)(unaff_SI + 1),uVar26);
      puVar24 = (uint *)(unaff_SI + 4);
      out(*(undefined2 *)(unaff_SI + 2),uVar26);
      iVar7 = CONCAT11((char)((uint)in_BX >> 8),(char)in_BX + (char)((uint)in_CX >> 8));
      bVar27 = *(byte *)((int)puVar24 + iVar7 + -0x6a53) < (byte)(uVar28 >> 0x10);
      do {
        uVar10 = (uint)uVar28;
        iVar12 = CONCAT11((char)(uVar28 >> 0x18),0x10);
        do {
          cVar11 = (char)((uint)in_CX >> 8);
          iVar12 = iVar12 + -1;
          uVar17 = unaff_BP >> 1;
          if (iVar12 == 0) {
            puVar2 = puVar24;
            puVar24 = puVar24 + 1;
            uVar17 = *puVar2;
            iVar12 = 0x10;
            uVar10 = unaff_BP >> 1;
          }
          uVar15 = (iVar7 << 1 | (uint)bVar27) << 1 | (uint)((unaff_BP & 1) != 0);
          if (1 < (byte)uVar15) goto LAB_1000_010b;
          uVar14 = uVar17 & 1;
          uVar18 = uVar17 >> 1;
          iVar12 = iVar12 + -1;
          uVar17 = uVar18;
          if (iVar12 == 0) {
            puVar2 = puVar24;
            puVar24 = puVar24 + 1;
            uVar17 = *puVar2;
            iVar12 = 0x10;
            uVar10 = uVar18;
          }
          uVar14 = uVar15 << 1 | (uint)(uVar14 != 0);
          uVar15 = 0;
          if (uVar14 == 0) {
LAB_1000_0104:
            uVar15 = CONCAT11((char)(uVar15 >> 8),*(undefined1 *)(uVar15 + 0x209));
LAB_1000_010b:
            uVar9 = (undefined1)(uVar10 >> 8);
            bVar13 = 0;
            if (uVar15 != 2) {
              uVar14 = uVar17 & 1;
              uVar18 = uVar17 >> 1;
              iVar12 = iVar12 + -1;
              uVar17 = uVar18;
              if (iVar12 == 0) {
                puVar2 = puVar24;
                puVar24 = puVar24 + 1;
                uVar17 = *puVar2;
                iVar12 = 0x10;
                uVar10 = uVar18;
              }
              uVar9 = (undefined1)(uVar10 >> 8);
              if (uVar14 == 0) {
                uVar18 = uVar17 & 1;
                iVar12 = iVar12 + -1;
                uVar14 = uVar17 >> 1;
                if (iVar12 == 0) {
                  puVar2 = puVar24;
                  puVar24 = puVar24 + 1;
                  uVar14 = *puVar2;
                  iVar12 = 0x10;
                  uVar10 = uVar17 >> 1;
                }
                iVar12 = iVar12 + -1;
                uVar21 = uVar14 >> 1;
                if (iVar12 == 0) {
                  puVar2 = puVar24;
                  puVar24 = puVar24 + 1;
                  uVar21 = *puVar2;
                  iVar12 = 0x10;
                  uVar10 = uVar14 >> 1;
                }
                uVar14 = (uint)((uVar14 & 1) != 0);
                iVar12 = iVar12 + -1;
                uVar17 = uVar21 >> 1;
                if (iVar12 == 0) {
                  puVar2 = puVar24;
                  puVar24 = puVar24 + 1;
                  uVar17 = *puVar2;
                  iVar12 = 0x10;
                  uVar10 = uVar21 >> 1;
                }
                uVar9 = (undefined1)(uVar10 >> 8);
                uVar21 = (uint)((uVar21 & 1) != 0);
                uVar18 = ((uint)(uVar18 != 0) << 1 | uVar14) << 1 | uVar21;
                if (1 < (byte)uVar18) {
                  uVar6 = uVar17 & 1;
                  uVar19 = uVar17 >> 1;
                  iVar12 = iVar12 + -1;
                  uVar17 = uVar19;
                  if (iVar12 == 0) {
                    puVar2 = puVar24;
                    puVar24 = puVar24 + 1;
                    uVar17 = *puVar2;
                    iVar12 = 0x10;
                    uVar10 = uVar19;
                  }
                  uVar9 = (undefined1)(uVar10 >> 8);
                  uVar6 = (uint)(uVar6 != 0);
                  uVar18 = uVar18 << 1 | uVar6;
                  if (7 < (byte)uVar18) {
                    uVar19 = uVar17 & 1;
                    uVar20 = uVar17 >> 1;
                    iVar12 = iVar12 + -1;
                    uVar17 = uVar20;
                    if (iVar12 == 0) {
                      puVar2 = puVar24;
                      puVar24 = puVar24 + 1;
                      uVar17 = *puVar2;
                      iVar12 = 0x10;
                      uVar10 = uVar20;
                    }
                    uVar9 = (undefined1)(uVar10 >> 8);
                    uVar19 = (uint)(uVar19 != 0);
                    uVar18 = uVar18 << 1 | uVar19;
                    if (0x16 < (byte)uVar18) {
                      uVar18 = uVar17 & 1;
                      uVar20 = uVar17 >> 1;
                      iVar12 = iVar12 + -1;
                      uVar17 = uVar20;
                      if (iVar12 == 0) {
                        puVar2 = puVar24;
                        puVar24 = puVar24 + 1;
                        uVar17 = *puVar2;
                        iVar12 = 0x10;
                        uVar10 = uVar20;
                      }
                      uVar9 = (undefined1)(uVar10 >> 8);
                      bVar13 = (byte)((((uVar14 << 1 | uVar21) << 1 | uVar6) << 1 | uVar19) << 1) |
                               uVar18 != 0;
                      goto LAB_1000_013c;
                    }
                  }
                }
                bVar13 = *(byte *)(uVar18 + 0x22f);
              }
            }
LAB_1000_013c:
            uVar14 = *puVar24;
            uVar10 = CONCAT11(uVar9,(byte)uVar14);
            piVar16[-1] = (int)((int)puVar24 + 1);
            pbVar23 = unaff_DI + -CONCAT11(bVar13,(byte)uVar14);
            piVar16[-2] = unaff_DS;
            piVar16[-3] = unaff_ES;
            uVar26 = piVar16[-3];
            for (; uVar15 != 0; uVar15 = uVar15 - 1) {
              pbVar3 = unaff_DI;
              unaff_DI = unaff_DI + 1;
              pbVar4 = pbVar23;
              pbVar23 = pbVar23 + 1;
              *pbVar3 = *pbVar4;
            }
            unaff_DS = piVar16[-2];
            puVar25 = (uint *)piVar16[-1];
          }
          else {
            uVar15 = uVar17 & 1;
            uVar17 = uVar17 >> 1;
            iVar12 = iVar12 + -1;
            if (iVar12 == 0) {
              puVar2 = puVar24;
              puVar24 = puVar24 + 1;
              iVar12 = 0x10;
              uVar10 = uVar17;
              uVar17 = *puVar2;
            }
            uVar15 = uVar14 << 1 | (uint)(uVar15 != 0);
            if ((byte)uVar15 < 5) goto LAB_1000_0104;
            uVar14 = uVar17 & 1;
            uVar18 = uVar17 >> 1;
            iVar12 = iVar12 + -1;
            uVar17 = uVar18;
            if (iVar12 == 0) {
              puVar2 = puVar24;
              puVar24 = puVar24 + 1;
              uVar17 = *puVar2;
              iVar12 = 0x10;
              uVar10 = uVar18;
            }
            uVar18 = uVar15 << 1;
            uVar15 = uVar18 | uVar14 != 0;
            if ((byte)uVar15 < 0xd) goto LAB_1000_0104;
            uVar14 = uVar17 & 1;
            uVar21 = uVar17 >> 1;
            iVar12 = iVar12 + -1;
            uVar17 = uVar21;
            if (iVar12 == 0) {
              puVar2 = puVar24;
              puVar24 = puVar24 + 1;
              uVar17 = *puVar2;
              iVar12 = 0x10;
              uVar10 = uVar21;
            }
            uVar14 = (CONCAT11((char)(uVar18 >> 8),(byte)uVar15) & 0xff03) << 1 |
                     (uint)(uVar14 != 0);
            if (4 < (byte)uVar14) {
              uVar15 = uVar17 & 1;
              uVar18 = uVar17 >> 1;
              iVar12 = iVar12 + -1;
              uVar17 = uVar18;
              if (iVar12 == 0) {
                puVar2 = puVar24;
                puVar24 = puVar24 + 1;
                uVar17 = *puVar2;
                iVar12 = 0x10;
                uVar10 = uVar18;
              }
              uVar18 = uVar14 << 1;
              uVar14 = uVar18 | uVar15 != 0;
              if (0xc < (byte)uVar14) {
                uVar15 = uVar17 & 1;
                uVar21 = uVar17 >> 1;
                iVar12 = iVar12 + -1;
                uVar17 = uVar21;
                if (iVar12 == 0) {
                  puVar2 = puVar24;
                  puVar24 = puVar24 + 1;
                  uVar17 = *puVar2;
                  iVar12 = 0x10;
                  uVar10 = uVar21;
                }
                uVar15 = (CONCAT11((char)(uVar18 >> 8),(byte)uVar14) & 0xff03) << 1 |
                         (uint)(uVar15 != 0);
                if (4 < (byte)uVar15) {
                  uVar14 = uVar17 & 1;
                  uVar18 = uVar17 >> 1;
                  iVar12 = iVar12 + -1;
                  uVar17 = uVar18;
                  if (iVar12 == 0) {
                    puVar2 = puVar24;
                    puVar24 = puVar24 + 1;
                    uVar17 = *puVar2;
                    iVar12 = 0x10;
                    uVar10 = uVar18;
                  }
                  uVar15 = uVar15 << 1 | (uint)(uVar14 != 0);
                }
                uVar15 = CONCAT11(cVar11,*(undefined1 *)(uVar15 + 0x21f));
                goto LAB_1000_010b;
              }
            }
            uVar15 = CONCAT11(cVar11,*(char *)(uVar14 + 0x214));
            if (*(char *)(uVar14 + 0x214) != '\x19') goto LAB_1000_010b;
            puVar2 = puVar24;
            puVar24 = (uint *)((int)puVar24 + 1);
            bVar13 = (byte)*puVar2;
            uVar10 = CONCAT11((char)(uVar10 >> 8),bVar13);
            uVar15 = CONCAT11(cVar11 + (0xe6 < bVar13),bVar13 + 0x19);
            if (bVar13 < 0xfe) goto LAB_1000_010b;
            piVar16[-1] = uVar10;
            pbVar23 = unaff_DI + -0x2000;
            unaff_DI = (byte *)(((uint)unaff_DI & 0xf) + 0x2000);
            unaff_ES = unaff_ES + ((uint)pbVar23 >> 4);
            puVar25 = (uint *)((uint)puVar24 & 0xf);
            unaff_DS = unaff_DS + ((uint)puVar24 >> 4);
            uVar10 = piVar16[-1];
            if ((char)uVar10 == -1) {
              iVar7 = *piVar16 + 0x10;
              while( true ) {
                pbVar23 = (byte *)((int)puVar25 + 1);
                uVar10 = (uint)(byte)*puVar25;
                if (uVar10 == 0) break;
                puVar25 = (uint *)((int)puVar25 + 3);
                iVar12 = *(int *)pbVar23;
                do {
                  puVar2 = puVar25;
                  puVar25 = puVar25 + 1;
                  *(int *)*puVar2 = *(int *)*puVar2 + iVar7;
                  uVar10 = uVar10 - 1;
                } while (uVar10 != 0);
              }
              iVar8 = *(int *)pbVar23 + iVar7;
              iVar12 = *(int *)(byte *)((int)puVar25 + 3);
              *(int *)(iVar12 + -2) = iVar7 + *(int *)(byte *)((int)puVar25 + 5);
              *(undefined2 *)(iVar12 + -4) = *(undefined2 *)(byte *)((int)puVar25 + 7);
              return 0;
            }
          }
          while( true ) {
            uVar15 = uVar17 & 1;
            uVar17 = uVar17 >> 1;
            iVar12 = iVar12 + -1;
            puVar24 = puVar25;
            if (iVar12 == 0) {
              iVar12 = 0x10;
              uVar10 = uVar17;
              puVar24 = puVar25 + 1;
              uVar17 = *puVar25;
            }
            if (uVar15 != 0) break;
            pbVar4 = unaff_DI;
            unaff_DI = unaff_DI + 1;
            puVar25 = (uint *)((int)puVar24 + 1);
            *pbVar4 = (byte)*puVar24;
          }
          in_CX = 0;
          iVar7 = 0;
          bVar27 = (uVar17 & 1) != 0;
          iVar12 = iVar12 + -1;
          unaff_BP = uVar17 >> 1;
        } while (iVar12 != 0);
        puVar2 = puVar24;
        puVar24 = puVar24 + 1;
        unaff_BP = *puVar2;
        uVar28 = (ulong)(uVar17 >> 1);
      } while( true );
    }
  }
  else {
    iVar7 = 0x61f9;
    DAT_6000_23de = 0x61f9;
    in_CX = 0x123;
    unaff_DI = (byte *)0x0;
    DAT_6000_23dc = 0;
    unaff_ES = iVar7;
  }
  puVar22 = (undefined2 *)0x144;
  for (; in_CX != 0; in_CX = in_CX + -1) {
    pbVar4 = unaff_DI;
    unaff_DI = unaff_DI + 2;
    puVar1 = puVar22;
    puVar22 = puVar22 + 1;
    *(undefined2 *)pbVar4 = *puVar1;
  }
  return iVar7;
}


