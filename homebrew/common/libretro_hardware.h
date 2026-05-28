/*
 * Shared libretro-facing constants for the R36SX/SF3000-like firmware.
 *
 * Keep frontend/core cadence here instead of in hardware.h so native MIPS
 * programs can include only the device and firmware ABI constants.
 */

#ifndef R36SX_LIBRETRO_HARDWARE_H
#define R36SX_LIBRETRO_HARDWARE_H

#include "hardware.h"

/* Audio/video timing used by the small libretro demos. */
#define R36SX_LIBRETRO_VIDEO_FPS 60
#define R36SX_LIBRETRO_AUDIO_SAMPLE_RATE 44100
#define R36SX_LIBRETRO_AUDIO_FRAMES_PER_VIDEO_FRAME \
    (R36SX_LIBRETRO_AUDIO_SAMPLE_RATE / R36SX_LIBRETRO_VIDEO_FPS)

#endif
