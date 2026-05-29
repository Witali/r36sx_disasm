# disk_image_patch_086

Pico-286 HLT idle update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`

Behavior change:

- Opcode `F4` / `HLT` now sets a CPU halt state and returns from `exec86()`
  instead of continuing to execute the remaining host quantum.
- Later `exec86()` calls keep the guest halted until an unmasked PIC interrupt
  is pending and the guest interrupt flag is enabled.
- When the timer, keyboard, or another unmasked IRQ wakes the CPU, the existing
  `nextintr()` / `intcall86()` path services it normally.

Expected effect:

- Lower host CPU use in DOS prompts, BIOS idle loops, installers, and menus.
- Games that do not use `HLT` should behave the same.

Build details:

```text
pico_286 size: 902332 bytes
pico_286 SHA256: 3BE291DAC81BE5BA61F8FF7824C1870E8F301E9EFE07DA203F118069F8F2C88B
Defender scan: found no threats
```
