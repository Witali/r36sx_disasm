#ifndef R36SX_WINDOWS_PRINTF_RENAME_H
#define R36SX_WINDOWS_PRINTF_RENAME_H

/*
 * MinGW's CRT declares _vsnprintf.  The bundled tiny printf.c also uses that
 * identifier for an internal helper with a different signature.  Include this
 * only while compiling printf.c, after the common Windows compat header, so the
 * local helper and its call sites are renamed without touching CRT headers.
 */
#define _vsnprintf r36sx_tiny_printf_vsnprintf

#endif
