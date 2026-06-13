# x86 DOS/BIOS Binary Format Checks

## `.COM`

- Load offset is normally `CS:0100h`; source should normally use `org 100h`.
- PSP occupies `CS:0000h..00FFh`.
- Near jumps/calls and absolute data references may depend on the `ORG`.
- A `.COM` file has no relocation table. Do not introduce assumptions that
  require relocation.

## MZ `.EXE`

- Inspect the MZ header before patching: header size, relocation count, initial
  CS:IP, SS:SP, min/max allocation, and overlay number.
- Code offsets in disassembly are not always raw file offsets; account for the
  header and relocation table.
- If changing code size, check all near/far targets and relocation entries.

## Boot Sectors

- BIOS loads the sector at `0000:7C00h` or equivalent physical address.
- Keep the `55 AA` signature at offsets `510..511`.
- Preserve BPB fields unless the task is explicitly changing disk format.
- Keep total size exactly 512 bytes for a single boot sector.

## BIOS and Option ROMs

- Preserve ROM size, entry point, reset vector targets, tables, and checksums.
- PC BIOS reset normally starts through the CPU reset vector at physical
  `FFFF0h`, then jumps into BIOS code.
- Option ROMs start with `55 AA`; size is encoded in 512-byte units at byte 2.
  The checksum over the declared ROM size should be valid when required.

## Patch Verification

- Compare old/new binaries before and after each patch.
- Keep a note of changed offsets and why they changed.
- Use byte-identical rebuild checks before replacing hand-understood ranges in
  a ROM reconstruction.
