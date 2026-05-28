MIPS_NATIVE
===========

This directory is reserved for standalone MIPS native programs launched
directly from Tiny MC.

Layout convention:

MIPS_NATIVE/
  program_name/
    program_name
    README.txt
    data/

Rules:

- Put each native program in its own directory.
- Name the directory after the program.
- Name the main executable after the directory when practical.
- Keep program data next to the executable, inside the same directory.
- Tiny MC runs executable files directly with fork() + execl().
- Tiny MC runs files with a .sh suffix through /bin/sh.

Example:

MIPS_NATIVE/
  hello/
    hello
    README.txt

