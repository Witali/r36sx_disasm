# DOSBox CPU Core Comparison

## Source Download

Official upstream checked:

- Project page: <https://sourceforge.net/projects/dosbox/>
- Official SourceForge SVN tree: <https://sourceforge.net/p/dosbox/code-0/HEAD/tree/>
- Official 0.74-3 source release page:
  <https://sourceforge.net/projects/dosbox/files/dosbox/0.74-3/>

The SourceForge SVN snapshot endpoint returned an HTML interstitial in this
environment, so the local comparison uses the official 0.74-3 release source
tarball from SourceForge Files.

Downloaded archive:

- Local path:
  `internet_sources/dosbox_official/dosbox-0.74-3.real.tar.gz`
- Extracted path:
  `internet_sources/dosbox_official/dosbox-0.74-3`
- SHA256:
  `C0D13DD7ED2ED363B68DE615475781E891CD582E8162B5C3669137502222260A`
- Malware scan:
  `tools/scan-download.ps1` / Microsoft Defender found no threats.

## High-Level Architecture

Our current Pico-286 port is a compact single C interpreter centered on:

- `homebrew/pico_286/r36sx_port/r36sx_cpu.c`
- `homebrew/pico_286/pico-286/src/emulator/cpu.h`
- `homebrew/pico_286/pico-286/src/emulator/emulator.h`

The relevant local CPU/header files are about 178 KB total.

DOSBox has a much larger CPU subsystem in `src/cpu`, about 1.17 MB of source
files.  It has several interchangeable cores:

- `core_simple.cpp`
- `core_normal.cpp`
- `core_full.cpp`
- `core_prefetch.cpp`
- `core_dyn_x86.cpp`
- `core_dynrec.cpp`

The normal DOSBox core uses table-like include files for opcode families:

- `core_normal/prefix_none.h`
- `core_normal/prefix_0f.h`
- `core_normal/prefix_66.h`
- `core_normal/prefix_66_0f.h`

This is structurally cleaner than our monolithic switch.  It also means DOSBox
can represent 16-bit, 32-bit, and two-byte opcodes without special local hacks
inside each individual instruction.

## Opcode Dispatch

DOSBox handles `0F` as a true two-byte opcode prefix:

```cpp
CASE_B(0x0f) /* 2 byte opcodes */
    core.opcode_index|=OPCODE_0F;
    goto restart_opcode;
```

`66` and `67` are also true prefixes:

```cpp
CASE_B(0x66) /* Operand Size Prefix */
    core.opcode_index=(cpu.code.big^0x1)*0x200;
    goto restart_opcode;

CASE_B(0x67) /* Address Size Prefix */
    DO_PREFIX_ADDR();
```

Our Pico-286 core currently treats these as partial local state:

- `operandSizeOverride`
- `addressSizeOverride`
- compile-time `CPU_386_EXTENDED_OPS`

Only a small subset of instructions has 32-bit operand paths, so our existing
386 support is a partial real-mode compatibility stub rather than a complete
80386 core.

## CPUID Behavior

DOSBox implements `0F A2` in `core_normal/prefix_0f.h`:

```cpp
CASE_0F_B(0xa2) /* CPUID */
    if (!CPU_CPUID()) goto illegal_opcode;
```

`CPU_CPUID()` returns false for CPU types below `486_new`, so DOSBox raises an
invalid-opcode exception on 386:

```cpp
if (CPU_ArchitectureType<CPU_ARCHTYPE_486NEWSLOW) return false;
```

This matches our recent fix: for 286-class behavior, `0F A2` must not silently
execute.  It should raise `INT 6`.  If we later add a `cpu_model=80386`
runtime option, CPUID should still be invalid there.

## CPU Type Model

DOSBox has explicit runtime CPU architecture types:

```cpp
#define CPU_ARCHTYPE_MIXED       0xff
#define CPU_ARCHTYPE_386SLOW     0x30
#define CPU_ARCHTYPE_386FAST     0x35
#define CPU_ARCHTYPE_486OLDSLOW  0x40
#define CPU_ARCHTYPE_486NEWSLOW  0x45
#define CPU_ARCHTYPE_PENTIUMSLOW 0x50
```

The config `cputype` maps onto these values in `cpu.cpp`:

- `auto`
- `386`
- `386_prefetch`
- `386_slow`
- `486_slow`
- `486_prefetch`
- `pentium_slow`

Our port currently has only a speed knob (`cpu_mhz`) and compile-time CPU
feature defines.  A useful next step is to add a runtime `cpu_model` field and
use it to gate 8086/286/386/486 behavior.

## Protected Mode and Paging

DOSBox has real protected-mode state and control-register handling:

- `CPU_SET_CRX()` switches CR0 protected mode and paging.
- `CPU_LGDT()`, `CPU_LIDT()`, selector loading, far calls, and far jumps are
  routed through CPU helpers.
- `paging.cpp` implements page tables, TLB links, page faults, and CR3.

Our Pico-286 port does not have equivalent protected-mode/paging machinery.
This is the main reason that "upgrade to 386" is not just adding 32-bit
register operations.

## HLT and Cycle Accounting

DOSBox HLT:

```cpp
void CPU_HLT(Bitu oldeip) {
    reg_eip=oldeip;
    CPU_Cycles=0;
    cpu.hlt.cs=SegValue(cs);
    cpu.hlt.eip=reg_eip;
    cpu.hlt.old_decoder=cpudecoder;
    cpudecoder=&HLT_Decode;
}
```

Our port now has `hltstate` and exits `exec86()` until a maskable interrupt is
pending.  That is conceptually close, but DOSBox also integrates HLT with its
decoder switching and cycle scheduler.

DOSBox string operations also consume cycles in batches and preserve remaining
work.  Our current REP batching is simpler and bounded by
`R36SX_REP_BATCH_MAX`.

## Practical Lessons for Pico-286

Recommended changes to borrow from DOSBox conceptually:

1. Convert `0F`, `66`, and `67` into real prefix/decode state, not ad-hoc
   per-instruction flags.
2. Add a runtime CPU architecture enum similar to DOSBox
   `CPU_ArchitectureType`.
3. Keep CPUID invalid for 8086/286/386; only enable it for 486-new/Pentium-like
   modes if we intentionally add those modes.
4. Split the monolithic CPU file into opcode-family headers or helper modules
   before attempting broad 386 support.
5. Implement 386 real-mode support first: complete 32-bit operand/address
   forms, PUSHFD/POPFD/IRETD, 32-bit stack ops, and common `0F` bit-test/movzx
   instructions.
6. Treat protected mode and paging as a separate milestone.  DOSBox's
   `cpu.cpp` and `paging.cpp` show that this is a subsystem, not an opcode
   patch.

## Bottom Line

DOSBox is not just a larger opcode table.  Its CPU emulation is built around
CPU type, execution core selection, protected-mode segment semantics, paging,
exceptions, and cycle scheduling.  Our Pico-286 core is much smaller and easier
to tune for the R36SX, but it will need a structural decode refactor before a
clean 386 upgrade makes sense.
