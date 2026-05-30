# Disk Image Patch 101

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 with the computed-goto CPU opcode dispatcher:

- the default build enables `R36SX_CPU_COMPUTED_GOTO=1`;
- the main `exec86()` decoder jumps through a 256-entry GNU labels-as-values
  table before the original switch body;
- the old switch decoder remains available by rebuilding with
  `build_pico_286.ps1 -DisableComputedGoto`.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

```text
pico_286 size: 982416 bytes
pico_286 SHA256: 880BF4A3BC3387A22F0D034AC644F238D540A9AE71409750D4423BC337CBE914
```
