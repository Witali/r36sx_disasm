# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/keypresets.conf`
- `MIPS_NATIVE/pico_286/FreeDOS1.img`
- `MIPS_NATIVE/pico_286/FreeDOS2.img`
- `MIPS_NATIVE/pico_286/FreeDOS3.img`
- `MIPS_NATIVE/pico_286/sopwith.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

This debug build writes a startup/runtime log to:

- `MIPS_NATIVE/pico_286/pico_286.log`

If that path cannot be opened on the device, it falls back to:

- `pico_286.log` in the SD-card root

## 2026-05-29 boot order and HDD geometry config

`pico_286.conf` now includes:

```ini
boot_order=fdd0,hdd0
hdd0_geometry=65,16,63
hdd1_geometry=65,16,63
```

`boot_order` accepts `fdd0`, `fdd1`, `hdd0`, and `hdd1`; `boot_order=rom`
keeps the embedded ROM BIOS boot path.  The R36SX `INT 19h` hook probes boot
sectors in the configured order, checks for the `55 AA` signature, loads the
selected sector to `0000:7C00`, and sets `DL` to the selected BIOS drive.

`hdd0_geometry` and `hdd1_geometry` are CHS overrides in
`cylinders,heads,sectors` order.  The bundled hard disk images match
`65,16,63`.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8037084 bytes
SHA256: DB5654656F9C97D19557E63E38E46D8A5E4A604916C4F52092670C5698088B58
Defender scan: found no threats
```

## 2026-05-29 BIOS boot prompt mode

`pico_286.conf` now includes:

```ini
boot_mode=normal
```

`normal` attaches the configured disks during BIOS `INT 19h` and boots DOS.
`bios_prompt` leaves disks detached at `INT 19h`, so the embedded Turbo XT BIOS
stops at its boot prompt.  The ROM contains boot/diagnostic text, but no full
interactive CMOS/BIOS setup utility.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8016928 bytes
SHA256: 4AF76508FCF896130572E53E0D4B1A6E3BF5060FAB02817888CDFF17FE4194AD
Defender scan: found no threats
```

## 2026-05-29 configurable CPU speed

`pico_286.conf` now includes:

```ini
cpu_mhz=32.768
```

This controls the R36SX host execution quantum passed to `exec86()`.
`32.768` preserves the previous hard-coded behavior (`exec86(32768)` per
millisecond-like host loop).  The setting is a practical speed knob for this
port, not a cycle-exact 80286 clock.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8010412 bytes
SHA256: FE190E432D63957A91728F0E9BCEC7895BF5ECE7FC3D99291D0211E10BD84B43
Defender scan: found no threats
```

Optional PC disk images should be placed in the same directory:

- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/FreeDOS1.img`
- `MIPS_NATIVE/pico_286/FreeDOS2.img`
- `MIPS_NATIVE/pico_286/sopwith.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`
- `MIPS_NATIVE/pico_286/FreeDOS3.img` for older test builds that request a third
  floppy image

Button presets are stored in:

- `MIPS_NATIVE/pico_286/keypresets.conf`

The default preset matches the original direct DOS mapping:

```ini
active=Default

[preset Default]
up=UP
down=DOWN
left=LEFT
right=RIGHT
a=ENTER
b=ESC
y=CTRL
x=SPACE
start=ENTER
l=ALT
l2=F1
r=SHIFT
r2=F2
```

When DOS is running and the on-screen keyboard is hidden, Select opens or
closes the full-screen preset editor.  D-pad chooses a row.  The current editor
uses a two-column physical button list, opens the shared on-screen keyboard as
a picker for key assignments and preset renames, and saves only through the
visual `OK` button.  `Cancel` or Select closes without saving the draft.

The included image set is based on official FreeDOS 1.4 Floppy Edition:

- `FreeDOS1.img`: `144m/x86BOOT.img`, bootable FreeDOS floppy.
- `FreeDOS2.img`: `144m/x86DSK01.img`.
- `FreeDOS3.img`: `144m/x86DSK02.img`.
- `hdd.img` and `hdd2.img`: 33,546,240-byte hard disk images with MBR and one
  FAT16 primary partition, 65/16/63 CHS.

The downloaded FreeDOS archive, extracted files, and final copied image
directories were scanned with `tools/scan-download.ps1`; Microsoft Defender
reported no threats.

The current patch also includes a separate Sopwith (The Author's Edition)
game floppy:

- `sopwith.img`: 1.44 MB FAT12 floppy with `SW.EXE`, `SOPWTH.BAT`, and
  `README.TXT`.

`pico_286.conf` currently maps DOS `B:` / BIOS drive `01h` to this game
floppy:

```ini
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd1=hdd2.img
```

Sopwith was downloaded from DOS Games Archive (`swae-box.zip`, SHA256
`689BC2B80BFE9A7079F94C7A550BA2E0D7FA1842DC4F9C6BCED225A7424BAFBA`) and
packed with `tools/create_fat12_floppy.py`.  The ZIP, extracted files, and
all three final image copies were scanned with `tools/scan-download.ps1`;
Microsoft Defender reported no threats.

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
included `FreeDOS1.img` floppy image.

The current binary includes a joystick-controlled on-screen keyboard and a key
preset editor:

- Fn toggles the keyboard.
- Select opens the key preset editor when the keyboard is hidden.
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
cpu_mhz=32.768
boot_mode=normal
boot_order=fdd0,hdd0
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd0_geometry=65,16,63
hdd1=hdd2.img
hdd1_geometry=65,16,63
```

`cpu_mhz` controls the R36SX execution quantum passed to `exec86()`;
`32.768` keeps the previous hard-coded speed (`exec86(32768)` per
millisecond-like host loop).  It is a practical speed knob, not a cycle-exact
80286 timing model.

`boot_mode=normal` attaches the configured disks during BIOS `INT 19h` and
boots DOS.  `boot_mode=bios_prompt` leaves disks detached at `INT 19h`, so the
embedded Turbo XT BIOS stops at its boot prompt.  The ROM does not include a
full interactive CMOS/BIOS setup utility.

`boot_order` selects the boot sector probe order used by the R36SX `INT 19h`
hook.  Supported names are `fdd0`, `fdd1`, `hdd0`, and `hdd1`; use
`boot_order=rom` to chain to the embedded ROM BIOS boot path instead.

These map to BIOS drives `00h`, `01h`, `80h`, and `81h` respectively.  Paths
are relative to the Pico-286 directory unless absolute paths are used.  Empty
values disable a drive, and a missing config file falls back to the same four
default filenames.

`hdd0_geometry` and `hdd1_geometry` are optional CHS overrides in
`cylinders,heads,sectors` order.  The bundled hard disk images are
33,546,240 bytes, matching `65,16,63`.

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
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd1=hdd2.img
```

These map to BIOS drives `00h`, `01h`, `80h`, and `81h`.  Relative paths are
resolved from the directory that contains `pico_286.conf`.  Empty values
disable a drive.  If the config file is missing, Pico-286 falls back to these
legacy internal names: `fdd0.img`, `fdd1.img`, `hdd.img`, and `hdd2.img`.

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

## 2026-05-29 key preset editor

Added `MIPS_NATIVE/pico_286/keypresets.conf` and rebuilt Pico-286 with a
full-screen key preset editor.  Select opens or closes it while DOS is running
and the on-screen keyboard is hidden.  The editor can switch presets, copy the
active preset into a new automatically named preset, and assign each game
button to a supported keyboard key.

```text
Size: 8005860 bytes
SHA256: 583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5
Defender scan: found no threats
```

## 2026-05-29 FreeDOS image rename

The FreeDOS floppy images were renamed:

- `fdd0.img` -> `FreeDOS1.img`
- `fdd1.img` -> `FreeDOS2.img`
- `fdd2.img` -> `FreeDOS3.img`

`pico_286.conf` now boots DOS from `FreeDOS1.img` and keeps `sopwith.img`
mounted as DOS `B:`.  The binary keeps the legacy missing-config fallback names
inside the app.

```text
Size: 8005860 bytes
SHA256: 583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5
Defender scan: found no threats
```

## 2026-05-29 hard disk write flush fix

The disk layer was rebuilt so writes to `hdd.img` survive an in-emulator reboot
immediately after formatting DOS `C:`.

`hdd0=hdd.img` is mounted as BIOS drive `80h`, then normalized to internal disk
slot `2`.  Previously, `writedisk()` used `fwrite()` without `fflush()`, and
`ejectdisk()` did not close the old `FILE *` before `INT 19h` mounted the disk
again.  A freshly formatted `hdd.img` could therefore be reopened before the old
buffered writes reached the image.

The current binary flushes after sector writes, flushes/closes the image on
eject/remount, and fixes hard-disk/floppy counters to use normalized slots
`0`/`1` for floppies and `2`/`3` for hard disks.

```text
Size: 8006344 bytes
SHA256: 451570BAC02DBADDF73AC3A9B2EB6CF96A9A31C1E2943ADC766C8A6A455F03EC
Defender scan: found no threats
```

## 2026-05-29 BIOS hard disk registration

The current binary also reports inserted hard disks through BIOS-visible state
for DOS/FDISK:

- updates BIOS Data Area byte `0040:0075` with the fixed-disk count,
- answers `INT 13h AH=10` and `AH=11` for ready/recalibrate checks,
- answers `INT 13h AH=15` with hard-disk type `0x03` and `CX:DX` total sectors.

Pico-286 has no interactive BIOS setup screen.  The hard disk geometry is
derived from image size when `hdd.img` is inserted.  For the included
33,546,240-byte image the geometry is 65 cylinders, 16 heads, 63 sectors.

```text
Size: 8006884 bytes
SHA256: 0737E66B6DB7C0165F3A1B22BBF3200678CEA8E7BDC339A827DA56392234ECBF
Defender scan: found no threats
```

## 2026-05-29 key preset editor draft UI

The Pico-286 binary was rebuilt after reworking the Select key preset editor.
The binding list now uses two columns so all physical buttons stay inside the
outer frame.  Opening the editor starts a draft session: choosing another
preset, adding a preset, renaming it, or changing bindings does not touch
`keypresets.conf` until the visual `OK` button is selected.  `Cancel` and
`Select` close the editor without saving the draft.

The shared on-screen keyboard now has a picker mode.  Pico-286 uses it for
renaming presets and for choosing the key assigned to each physical button row.

```text
Size: 8054724 bytes
SHA256: 598071628DA6ECA5B608625CB636AEF76977BB5C59D50039B4E93BB37FC2E4E1
Defender scan: found no threats
```

## 2026-05-29 FAT16 hard disk images and disk read diagnostics

The previous `hdd.img` and `hdd2.img` files were blank raw images: sector 0 had
no MBR, no BPB, and no `55 AA` signature.  On the device this can surface as
DOS `DIR C:` reporting `Cannot read` / retry / abort because BIOS drive `80h`
exists but the backing image is not partitioned or formatted.

Both hard disk images are now generated on the host with
`tools/create_fat16_hdd.py`:

- CHS: `65,16,63`
- MBR primary partition: active FAT16 type `0x06`
- partition start: sector `63`
- partition size: `65457` sectors
- VBR: FAT16, `63` sectors per track, `16` heads, hidden sectors `63`
- root directory contains `README.TXT` so `DIR C:` should show at least one
  file immediately.

Pico-286 was also rebuilt with extra `INT 13h` diagnostics.  Failed reads,
writes, verifies, invalid CHS requests, out-of-range transfers, `fseek`,
`fread`, `fwrite`, and `fflush` failures are written to
`/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log` in debug builds.

```text
pico_286 size: 8058240 bytes
pico_286 SHA256: 43E4C5A227FE8E83F4A643F1546F6E9B51E79D509B78BA3E13C4B1C75CDC0332
hdd.img SHA256: 26953A16F571AB8452570E37E2C0688C0B60A2859E72B81D6E9EBA8D80379818
hdd2.img SHA256: 45A095789D0C5A4F8E0BB2717493874C761789AF199D602BF794DBAB12448C71
Defender scan: found no threats
```
