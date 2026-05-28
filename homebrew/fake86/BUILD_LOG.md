# Fake86 Build Log

## Source

The Fake86 source was downloaded with:

```powershell
git clone https://github.com/lgblgblgb/fake86.git .\internet_sources\fake86
```

Downloaded content was scanned before use:

```powershell
.\tools\scan-download.ps1 .\internet_sources\fake86
```

No malware was reported by the scanner.

## Build

The R36SX build is produced with:

```powershell
.\homebrew\fake86\build_fake86.ps1
```

The build script:

- copies upstream `src/*.c` into `homebrew/fake86/obj/src`;
- patches the generated copy of `config.h` to disable KVM and OSD;
- patches the generated copy of `main.c` to add default console arguments and
  log redirection;
- patches `render.c` in the generated copy so OSD code is not referenced;
- compiles upstream Fake86 C files with the local MIPS toolchain;
- compiles the local `r36sx_sdl.c` SDL compatibility layer;
- links a native MIPS executable named `homebrew/fake86/fake86`.

Default no-argument launch command:

```text
fake86 -boot rom -nosound -speed 4770000 -delay 16
```

The native SDL shim logs early `SDL_RenderPresent()` calls to help distinguish
an emulator-side black frame from a display-driver problem.

Current output:

```text
homebrew/fake86/fake86
size: 441616 bytes
```

## Install Commands Used

The binary and BIOS/data files were copied into the working disk image:

```powershell
New-Item -ItemType Directory -Force .\disk_image\MIPS_NATIVE\fake86\data
Copy-Item -LiteralPath .\homebrew\fake86\fake86 -Destination .\disk_image\MIPS_NATIVE\fake86\fake86 -Force
Copy-Item -LiteralPath .\internet_sources\fake86\bin\data\asciivga.dat, .\internet_sources\fake86\bin\data\pcxtbios.bin, .\internet_sources\fake86\bin\data\rombasic.bin, .\internet_sources\fake86\bin\data\videorom.bin -Destination .\disk_image\MIPS_NATIVE\fake86\data -Force
```

The same files were copied into the redistributable patch folder:

```powershell
New-Item -ItemType Directory -Force .\patches\disk_image_patch_fake86\MIPS_NATIVE\fake86\data
Copy-Item -LiteralPath .\homebrew\fake86\fake86 -Destination .\patches\disk_image_patch_fake86\MIPS_NATIVE\fake86\fake86 -Force
Copy-Item -LiteralPath .\internet_sources\fake86\bin\data\asciivga.dat, .\internet_sources\fake86\bin\data\pcxtbios.bin, .\internet_sources\fake86\bin\data\rombasic.bin, .\internet_sources\fake86\bin\data\videorom.bin -Destination .\patches\disk_image_patch_fake86\MIPS_NATIVE\fake86\data -Force
```

## Notes

The first target is a minimal "shows something and logs failures" build. The
next useful step is to run it from TinyMC on hardware and inspect the three log
files if it returns to the launcher.
