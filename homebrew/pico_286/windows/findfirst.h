#ifndef R36SX_WINDOWS_FINDFIRST_H
#define R36SX_WINDOWS_FINDFIRST_H

/*
 * The POSIX host build uses the bundled findfirst compatibility library.
 * Native Windows already provides _findfirst/_findnext/_findclose and the
 * matching _finddata_t in the CRT, so expose those directly to the redirector.
 */
#include <io.h>

#endif
