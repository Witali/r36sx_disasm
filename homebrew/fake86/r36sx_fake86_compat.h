/*
 * Forced include for compiling Fake86+ with Clang/Zig in C11 mode.
 */

#ifndef R36SX_FAKE86_COMPAT_H
#define R36SX_FAKE86_COMPAT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#endif
