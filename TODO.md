# TODO

## Native Audio Stream Stability

- Update `homebrew/common/driver_audio.h` so native applications send silence
  to `sound_driver_playframe()` every frame, not only short effect sounds.
- `driver.so` appears to be designed for a continuous 44.1 kHz stereo stream,
  so a steady silent frame stream may make audio behavior more stable.
- After changing the helper, rebuild and test at least:
  - `homebrew/native_button_demo`
  - `homebrew/pong`
  - `homebrew/tiny_mc`

## Button Demo Exit Shortcut

- Add the same native exit shortcut used by Pong: `SELECT + START`.
- Keep or reconsider the current `FN` exit behavior after hardware testing.
- Rebuild Button Demo and update the relevant `disk_image` and patch copies.

## Shared Native Video Helper

- Extract the repeated `driver.so` video setup into a common helper/header.
- Candidate repeated logic:
  - `dlopen()` / `dlsym()` for `driver.so`
  - `video_driver_setting()`
  - `video_drivers_init()`
  - `video_driver_disp_frame()`
  - `video_driver_deinit()`
  - joypad `cube_ioctl()` setup where it naturally belongs
- Replace duplicated video initialization code in native applications with the
  shared helper after one app proves the interface.
