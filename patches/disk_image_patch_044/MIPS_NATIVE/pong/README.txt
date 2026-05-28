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
- Select or Fn: exit back to Tiny MC.

If the right paddle is not controlled manually, a small built-in AI follows the
ball. The left player wins when the left score reaches 9. If the right side
reaches 9 first, the game shows YOU LOSE.

The game plays generated bounce, score, pause, win, and lose sounds through
/dev/auddec. Audio follows the same packet model used by stock hcprojector UI
WAV effects, but generates PCM internally and does not require external WAV
files.
