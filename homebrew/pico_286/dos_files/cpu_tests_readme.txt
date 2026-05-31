R36SX Pico-286 CPU test disk
============================

Files:

ID.COM
  Small PCjs CPU identification test. It prints detected CPU class,
  machine status word, IDTR, and CR0 where supported.

TEST386.COM
  PCjs 80386 CPU/protected-mode test program. It can run as a DOS COM
  program or as a ROM image in PCjs. On Pico-286, run it from DOS after
  booting FreeDOS.

TEST386.BIN
  R36SX build of barotto/test386.asm. This is a 64KB BIOS replacement test
  ROM, not a DOS COM program. It is stored here as a test payload/reference.
  The source is in homebrew/pico_286/tests/test386.asm. This build enables
  DEBUG, writes ASCII output to port 0191h, and writes POST codes to the
  standard POST port 0080h. The Pico-286 port logs those ports to
  pico_286.log. It also writes short VGA breadcrumbs at B800:0000 during
  early POST 01 tests: JCC8, JCC16, LOOP, LOOPZ, and LOOPNZ.

CPUID.ASM
ID.ASM
  Source files from PCjs for reference.

Suggested use:

1. Boot FreeDOS from A:.
2. Mount this image as B: with the Pico-286 disk menu, or set fdd1 to
   cpu_tests.img in pico_286.conf.
3. Run:

   B:
   ID
   TEST386

4. TEST386.BIN is not launched from DOS. To execute it, the emulator must load
   it as the BIOS/test ROM at physical F0000h and reset into its reset vector.

Notes:

SingleStepTests 8086/80286/80386 are not DOS COM programs. They are
hardware-recorded instruction test vectors for a host-side test runner, so
they are not stored on this DOS floppy image.

Sources:

PCjs CPU tests:
https://www.pcjs.org/software/pcx86/test/cpu/

SingleStepTests:
https://github.com/SingleStepTests

barotto/test386.asm:
https://github.com/barotto/test386.asm
