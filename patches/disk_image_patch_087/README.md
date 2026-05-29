# disk_image_patch_087

Pico-286 ticks thread sleep update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`

Behavior change:

- `R36SX_TICKS_THREAD_SLEEP_US` is now `1000u` instead of `250u`.
- The ticks thread still catches up timer, audio, Sound Blaster, Disney Sound
  Source, cursor blink, and video frame events in batches.
- Scheduler wakeups should drop from up to about 4000 per second to about 1000
  per second.

Expected effect:

- Lower Linux scheduler overhead while idle or lightly loaded.
- Audio is batched at roughly 44 output samples per 1 ms wakeup.

Build details:

```text
pico_286 size: 902332 bytes
pico_286 SHA256: CF3AD6C1C68D5640F9C4EE2C59C62ECF85A1DB39001EFD7E005C4D596BDE89F0
Defender scan: found no threats
```
