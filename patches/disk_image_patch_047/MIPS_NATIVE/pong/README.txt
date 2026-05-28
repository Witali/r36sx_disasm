Native Pong
-----------

Standalone MIPS Pong executable for Tiny MC.

Main executable:

  pong

Controls:

- D-pad up/down: left paddle.
- A/X: right paddle up.
- B/Y: right paddle down.
- Start: pause.
- A while paused: reset.
- A or Start after YOU WIN / YOU LOSE: new game.
- Select + Start together: exit back to Tiny MC.
- Fn: secondary hardware escape back to Tiny MC.

If the right paddle is not controlled manually, a small built-in AI follows the
ball. The left player wins when the left score reaches 9. If the right side
reaches 9 first, the game shows YOU LOSE.

This native build uses /mnt/sdcard/cubegm/driver.so for display, controls, and
audio. Bounce, score, pause, win, and lose sounds are generated locally and sent
through sound_driver_playframe, matching the stock rkgame LibRetro audio path.
