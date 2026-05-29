# disk_image_patch_088

Pico-286 REP MOVS/STOS batching update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`

Behavior change:

- `REP MOVSB`, `REP MOVSW`, `REP STOSB`, and `REP STOSW` now process up to
  1024 byte/word elements per decoded REP instruction.
- The batched path still uses the existing memory handlers, preserving RAM,
  VRAM, and mapped memory behavior.
- Direction flag movement, 16-bit `SI`/`DI` wraparound, and partial `CX`
  progress are preserved.
- When Trap Flag is active, the CPU falls back to one element at a time.

Expected effect:

- Faster DOS screen clears and repeated memory copies.
- Less CPU time spent repeatedly decoding the same REP prefix and string
  opcode.

Build details:

```text
pico_286 size: 908356 bytes
pico_286 SHA256: 19C64013150F4A659BE965DCD8D12045E00CC30F7C3639E6FBDE33FD46137DF7
Defender scan: found no threats
```
