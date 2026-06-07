# Sound Blaster Emulation TODO

This file tracks known gaps between the current Pico-286 Sound Blaster emulation
and the documented Sound Blaster 1.x/2.0 DSP behavior. The immediate compatibility
target is Supaplex with Sound Blaster FX enabled.

References:

- Creative Developer Kit for Sound Blaster Series, Second Edition:
  https://www.ardent-tool.com/sound/Dev_Kit_SB_Series_Hardware_Programming_Ref_DSA0085515.pdf
- DOSBox-X Sound Blaster DSP command notes:
  https://dosbox-x.com/wiki/Hardware%3ASound-Blaster%3ADSP-commands
- Programming Digitized Sound On the Sound Blaster:
  https://www.gamedeveloper.com/programming/programming-digitized-sound-on-the-sound-blaster

## Supaplex Notes

- `BLASTER.SND` uses normal 8-bit DMA playback commands, but also reaches DSP
  command `E2h` for DMA identification and command `24h` for 8-bit DMA input.
- The screen corruption seen with Sound Blaster FX is consistent with incorrect
  DSP/DMA side effects writing into guest memory.
- Commit `e1e08f81` made ADC DMA inert when no recording source is emulated and
  removed unguarded DMA writes/log spam from that path. More correctness work is
  still needed.

## High Priority

- [x] Add a Sound Blaster IRQ latch.
  - Set `irq_pending` whenever the DSP raises the configured 8-bit IRQ.
  - Reading DSP read-buffer/IRQ status at `base + 0Eh` must acknowledge the
    8-bit DSP IRQ and clear the latch.
  - Keep PIC EOI handling separate; DOS interrupt handlers normally read the SB
    ack port first, then send EOI to the PIC.

- [x] Fix DMA direction checks for both directions.
  - `i8237_read()` should only transfer when the DMA channel is enabled,
    unmasked, and configured for memory-to-device playback.
  - `i8237_write()` should only transfer when the channel is enabled, unmasked,
    and configured for device-to-memory recording or DMA identification writes.
  - Confirm the local `transfer_type` values against the 8237 mode register bits.

- [x] Model DSP recording commands safely.
  - `20h` direct ADC currently returns silence, which is acceptable as a stub.
  - `24h` single-cycle DMA input and `2Ch` auto-init DMA input should either
    write silence only through a valid device-to-memory DMA setup or stay inert.
  - They must not write guest memory from the audio callback unless the DMA state
    proves this is a real recording transfer.

## Medium Priority

- [x] Implement `D8h` Get Speaker Status.
  - After command `D8h`, the DSP should return `FFh` when speaker output is on
    and `00h` when it is off.

- [x] Improve `base + 0Ch` Write-Buffer Status.
  - Bit 7 should indicate DSP write-buffer busy status.
  - Returning always-ready is usually tolerated, but it is not hardware-accurate
    and can hide timing bugs.

- [x] Review auto-init exit/pause timing.
  - `D0h` pauses 8-bit DMA requests.
  - `D4h` resumes 8-bit DMA.
  - `DAh` should exit 8-bit auto-init at the end of the current block, not
    necessarily stop immediately.

- [x] Verify `E2h` DMA identification behavior.
  - The command should produce the expected byte through the selected 8-bit DMA
    channel when the guest has programmed a valid DMA write transfer.
  - It should not cause side effects when DMA is masked or configured for the
    wrong direction.

## Compatibility Cleanup

- Support Sound Blaster base `240h` or make the conflict explicit.
  - The Creative documentation lists `220h` and `240h` for SB2.0.
  - The current port map uses `240h..24Fh` for XT RTC, so SB is effectively
    only available at `220h`.

- Separate CMS/GameBlaster and DSP behavior more clearly.
  - SB 1.x/2.0 maps CMS ports at `base + 0h..3h` when CMS chips exist.
  - The current write path intentionally falls through from CMS ports to the SB
    handler, which is mostly harmless because the DSP ignores those offsets, but
    it is easy to misunderstand and should be made explicit.

- Add optional debug tracing for DSP commands.
  - Trace reset, command bytes, DMA command start/end, IRQ latch changes, and DMA
    direction decisions behind a debug define.
  - Keep tracing off by default to avoid audio-thread stalls.

- Add a small SB probe test.
  - Reset DSP and verify `AAh`.
  - Query `E1h` and verify version `2.01`.
  - Exercise `E0h`, `E2h`, `D1h`, `D3h`, `D8h`, `14h`, and IRQ ack behavior.
