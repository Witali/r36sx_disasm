# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/fdd0.img`
- `MIPS_NATIVE/pico_286/fdd1.img`
- `MIPS_NATIVE/pico_286/fdd2.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

This debug build writes a startup/runtime log to:

- `MIPS_NATIVE/pico_286/pico_286.log`

If that path cannot be opened on the device, it falls back to:

- `pico_286.log` in the SD-card root

Optional PC disk images should be placed in the same directory:

- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/fdd0.img`
- `MIPS_NATIVE/pico_286/fdd1.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`
- `MIPS_NATIVE/pico_286/fdd2.img` for older test builds that request a third
  floppy image

The included image set is based on official FreeDOS 1.4 Floppy Edition:

- `fdd0.img`: `144m/x86BOOT.img`, bootable FreeDOS floppy.
- `fdd1.img`: `144m/x86DSK01.img`.
- `fdd2.img`: `144m/x86DSK02.img`.
- `hdd.img` and `hdd2.img`: blank raw hard disk images, 65/16/63 CHS,
  33,546,240 bytes each.

The downloaded FreeDOS archive, extracted files, and final copied image
directories were scanned with `tools/scan-download.ps1`; Microsoft Defender
reported no threats.

The current binary is an unstripped ELF32 little-endian MIPS executable.  Zig
0.16.0 `objcopy --strip-all` reports `error: unimplemented` for this file, so
the build keeps symbols for now.

This patch was rebuilt after the first device log showed:

- `main: pthread_create sound=0 ticks=0`
- `main: fatal signal 11`

The new binary adds first-iteration logs for `sound_thread`, `ticks_thread`,
`exec86()`, and `mfb_update()`.  It also avoids the upstream audio cast from
`int16_t *` to `int32_t *` in `get_sound_sample()`.

Follow-up fix: the first diagnostic run reached `main: before exec loop=0` and
then crashed.  The Linux entrypoint had not initialized `read86`, `write86`,
`readw86`, `writew86`, `readdw86`, or `writedw86`; the new binary initializes
them to the same `*_ob` memory backend used by upstream `win32-main.cpp`.

Current build also includes an on-screen disk activity indicator: a small
blinking red circle appears in the lower-right corner when Pico-286 reads from
or writes to disk image sectors.

It also maps BIOS video modes `0x20` and `0x30` to the 80x25 text renderer so
FreeDOS/BIOS boot text can be shown instead of `unsupported video mode 20/30`.

The current build adds the missing text-mode BIOS `INT 10h` services needed by
BIOS/DOS boot screens, including cursor shape/position, scroll/clear window,
read/write character and attribute, teletype output, mode query, active page
selection, and write string.  It also fixes the normal text-mode renderer to
read from logical `VIDEORAM` cells so text printed by those BIOS calls can be
shown on screen.

The on-screen debug overlay has been disabled.  Diagnostics still go to
`MIPS_NATIVE/pico_286/pico_286.log`, but `DEBUG_VRAM` is no longer drawn into
the bottom of the framebuffer, and the emulated screen text is no longer dumped
to the log as `screen_text:*` entries.

Device testing confirmed that this patch set boots DOS successfully from the
included FreeDOS `fdd0.img` floppy image.

The current binary includes a joystick-controlled on-screen keyboard:

- Fn toggles the keyboard.
- D-pad moves the highlighted key.
- A or Start types the highlighted key.
- B sends Backspace immediately.
- Select closes the keyboard.
- X sends Escape immediately.
- Y sends Enter immediately.
- Select+Start still exits back to TinyMC.

F1-F10, Delete, common DOS punctuation, and latched Shift/Ctrl/Alt modifiers
are available from the on-screen keyboard.  Shift is available as the on-screen
`SHF` key.

The keyboard uses a compact bottom layout: 96 pixels tall, about 20% of the
640x480 framebuffer.  While it is visible, the DOS screen is vertically
compressed with a halftone-style area filter into the remaining 384 pixels
above the keyboard instead of being covered.

Disk image bindings are configurable through `MIPS_NATIVE/pico_286/pico_286.conf`:

```ini
fdd0=fdd0.img
fdd1=fdd1.img
hdd0=hdd.img
hdd1=hdd2.img
```

These map to BIOS drives `00h`, `01h`, `80h`, and `81h` respectively.  Paths
are relative to the Pico-286 directory unless absolute paths are used.  Empty
values disable a drive, and a missing config file falls back to the same four
default filenames.

## 2026-05-29 rebuild

The binary was rebuilt after moving the R36SX Pico-286 source patches out of
`build_pico_286.ps1` into explicit source files under
`homebrew/pico_286/r36sx_port/`.  Runtime behavior should be unchanged; the
build script now compiles those files directly instead of generating patched
copies in `obj/`.

```text
Size: 7936004 bytes
SHA256: CB9551333236CBEDEC8A173E212459D95B055A6FABAB6BC9DEC74BC1EF3A59DA
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard timing fix

The binary was rebuilt so the joystick-controlled on-screen keyboard sends a
PC-style make code first, holds it for about 80 ms, then sends the break code on
a later input poll.  This avoids losing key presses when DOS reads the emulated
keyboard controller after the old immediate key-down/key-up pair.

```text
Size: 7938128 bytes
SHA256: 51D01BF7DC78EC2C38E9428C7C42FDE3E0FDDAF42538D3984C8E488001B79E99
Defender scan: found no threats
```

## 2026-05-29 keyboard controller FIFO

The binary was rebuilt again to replace the temporary on-screen keyboard
delayed-release workaround with a proper FIFO behind emulated ports `0x60` and
`0x64`.  Key make/break scancodes are now queued, and reads from port `0x60`
pop one byte at a time while raising another IRQ1 if more bytes remain.

```text
Size: 7940088 bytes
SHA256: AB360DAE43112F5A4B72286E9BBE968C172A1F1241DBC86591ED0D6F5725681A
Defender scan: found no threats
```

## 2026-05-29 keyboard FIFO size reduction

The emulated keyboard controller FIFO was reduced from 32 scancode bytes to 8.
This is enough for the current joystick/on-screen keyboard path while keeping
the model smaller.

```text
Size: 7940104 bytes
SHA256: 4EB23B444826C36A60B734F4F66FEB58676DED6FF0FD9676FBE7D77F4EAD01B9
Defender scan: found no threats
```

## 2026-05-29 keyboard scancode pacing

The emulated keyboard controller now waits 1 ms before making each queued
scancode byte available through port `0x60`.  This approximates PS/2 keyboard
wire timing: one device-to-host byte is an 11-bit frame, and a 10-16.7 kHz
clock gives about 660-1100 us per scancode byte.

```text
Size: 7942728 bytes
SHA256: 7DF7757CB0A685ACF8BB5C09F9EAC292D6AD737AA0342CF67DB93F208144F9D3
Defender scan: found no threats
```

## 2026-05-29 reusable on-screen keyboard module

The Pico-286 binary was rebuilt after moving the joystick-controlled on-screen
keyboard implementation into the reusable shared module
`homebrew/common/r36sx_screen_keyboard.*`.  The device-facing keyboard behavior
should remain the same; Pico-286 now uses the shared module through a small
callback adapter.

```text
Size: 7948928 bytes
SHA256: 1C1BC7F76802DB1B2974704AB0B73E13473A5A335B83280BB73DE1A4C07E85B3
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard face-button remap

The Pico-286 binary was rebuilt after changing the shared on-screen keyboard
bindings.  While the keyboard is visible, `B` now sends Backspace immediately
and `Y` now sends Enter immediately.  `Select` closes the keyboard, and the
on-screen `CLOSE` key remains available.

```text
Size: 7949544 bytes
SHA256: B5A395D7A8CEDA18537659AC27339D950BD9F1E98C48061E6893073AD87BAE79
Defender scan: found no threats
```

## 2026-05-29 disk image config

Added `MIPS_NATIVE/pico_286/pico_286.conf` and rebuilt the binary so Pico-286
reads floppy and hard disk bindings from the config file before BIOS boot.

Default bindings:

```ini
fdd0=fdd0.img
fdd1=fdd1.img
hdd0=hdd.img
hdd1=hdd2.img
```

These map to BIOS drives `00h`, `01h`, `80h`, and `81h`.  Relative paths are
resolved from the directory that contains `pico_286.conf`.  Empty values
disable a drive.  If the config file is missing, Pico-286 falls back to these
same four filenames.

```text
Size: 7965128 bytes
SHA256: A63CD65C36BE12E56E59EFF677F5C298DE6BD701993DC6EBC0E004D5A1151623
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard X Escape binding

The Pico-286 binary was rebuilt after changing the shared on-screen keyboard
bindings.  While the keyboard is visible, the physical `X` button now sends
Escape immediately.  Shift remains available through the on-screen `SHF` key.

```text
Size: 7965676 bytes
SHA256: 91D85743E2A32C304407E8BFA1B1D92F732C2FA024F6E73CD4F9DE82C4B66048
Defender scan: found no threats
```
