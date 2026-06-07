# Sound Blaster Probe

`sb_probe.asm` builds a small DOS `.COM` program for probing the Pico-286
Sound Blaster DSP implementation from inside the emulator.

The probe currently checks:

- DSP reset response `AAh`;
- DSP version command `E1h`, expecting `2.01`;
- DSP identification command `E0h`;
- speaker enable/disable plus `D8h` speaker status;
- `E2h` DMA identification through DMA channel 1 write mode;
- `14h` single-cycle DMA playback completion, IRQ7 delivery, and IRQ acknowledge
  through `base + 0Eh`.

Build it with:

```powershell
.\homebrew\pico_286\tests\sound_blaster\build_sb_probe.ps1
```

The generated `SBPROBE.COM` is written to `tests/sound_blaster/build/`.
