#ifndef R36SX_DEBUG_CONFIG_H
#define R36SX_DEBUG_CONFIG_H

/*
 * Central compile-time debug policy for the R36SX Pico-286 port.
 *
 * Build scripts still control the coarse DEBUG flag.  The switches below let
 * us enable one noisy diagnostic area at a time with -DR36SX_DEBUG_*=1 without
 * scattering ad-hoc #define DEBUG_FOO blocks through the emulator.
 */

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef R36SX_ENABLE_PROFILING
#define R36SX_ENABLE_PROFILING 1
#endif

#ifndef R36SX_ENABLE_PROTECTED_MODE
#define R36SX_ENABLE_PROTECTED_MODE 1
#endif

#ifndef R36SX_DEBUG_STDOUT_CAPTURE
#define R36SX_DEBUG_STDOUT_CAPTURE DEBUG
#endif

#ifndef R36SX_DEBUG_INVALID_OPCODE_DUMP
#define R36SX_DEBUG_INVALID_OPCODE_DUMP DEBUG
#endif

#ifndef R36SX_DEBUG_PM_DIAG
#define R36SX_DEBUG_PM_DIAG DEBUG
#endif

#ifndef R36SX_DEBUG_PM_VERBOSE
#if DEBUG && (!defined(PICO_ON_DEVICE) || !PICO_ON_DEVICE)
#define R36SX_DEBUG_PM_VERBOSE 1
#else
#define R36SX_DEBUG_PM_VERBOSE 0
#endif
#endif

#ifndef R36SX_DEBUG_REDIRECTOR_TRACE
#define R36SX_DEBUG_REDIRECTOR_TRACE 0
#endif

#ifndef R36SX_DEBUG_REDIRECTOR_ERRORS
#define R36SX_DEBUG_REDIRECTOR_ERRORS DEBUG
#endif

#ifndef R36SX_DEBUG_KEYBOARD_TRACE
#define R36SX_DEBUG_KEYBOARD_TRACE DEBUG
#endif

#ifndef R36SX_DEBUG_TEST_BIOS_TRACE
#define R36SX_DEBUG_TEST_BIOS_TRACE DEBUG
#endif

#endif
