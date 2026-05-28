# Unified Pong Source

`pong.c` is the single source file for both Pong builds:

- `R36SX_PONG_TARGET=1`: libretro core for `rkgame`.
- `R36SX_PONG_TARGET=2`: native Tiny MC executable using `driver.so`.

The shared section owns the game state, scoring, button semantics, RGB565
framebuffer drawing, and audio-event decisions. Target sections only translate
input, present video, and route audio to the correct platform API.

Native Pong follows the native-app exit rule: hold `Select + Start` together to
leave the program. `Fn` is still accepted as a secondary hardware escape.
