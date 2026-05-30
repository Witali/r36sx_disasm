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

Notes:

SingleStepTests 8086/80286/80386 are not DOS COM programs. They are
hardware-recorded instruction test vectors for a host-side test runner, so
they are not stored on this DOS floppy image.

Sources:

PCjs CPU tests:
https://www.pcjs.org/software/pcx86/test/cpu/

SingleStepTests:
https://github.com/SingleStepTests
