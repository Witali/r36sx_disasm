/*
 * Manual DOSBox config.h for the R36SX/SF3000 MIPS Linux port.
 *
 * This mirrors the useful parts of DOSBox 0.74-3's autotools-generated
 * config.h, but keeps the first device build conservative: no OpenGL, no
 * networking, no screenshots, no dynamic CPU core, and no x86 FPU assembly.
 */

#ifndef R36SX_DOSBOX_CONFIG_H
#define R36SX_DOSBOX_CONFIG_H

#define VERSION "0.74-3-r36sx"

#define PACKAGE "dosbox"
#define PACKAGE_NAME "dosbox"
#define PACKAGE_STRING "dosbox 0.74-3-r36sx"
#define PACKAGE_TARNAME "dosbox"
#define PACKAGE_VERSION "0.74-3-r36sx"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_URL ""

#define C_DEBUG 0
#define C_CORE_INLINE 0
#define C_DIRECTSERIAL 0
#define C_DYNAMIC_X86 0
#define C_DYNREC 0
#define C_FPU 1
#define C_FPU_X86 0
#define C_IPX 0
#define C_MODEM 0
#define C_OPENGL 0
#define C_SET_PRIORITY 0
#define C_SSHOT 0
#define C_UNALIGNED_MEMORY 0

#ifdef MIPSEL
#undef MIPSEL
#endif

#define X86 0x01
#define X86_64 0x02
#define MIPSEL 0x03
#define ARMV4LE 0x04
#define POWERPC 0x05
#define UNKNOWN 0xff
#define C_TARGETCPU MIPSEL

#undef C_HEAVY_DEBUG
#undef C_SDL_SOUND
#undef C_X11_XKB

#define C_ATTRIBUTE_ALWAYS_INLINE 1
#define C_ATTRIBUTE_FASTCALL 0
#define C_HAS_ATTRIBUTE 1
#define C_HAS_BUILTIN_EXPECT 1
#define C_HAVE_MPROTECT 0

#define DIRENT_HAS_D_TYPE 1
#define ENVIRON_INCLUDED 1
#define ENVIRON_LINKED 1
#define HAVE_INTTYPES_H 1
#define HAVE_MEMORY_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_PWD_H 1
#define HAVE_REALPATH 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define LINUX 1
#define STDC_HEADERS 1

#define SIZEOF_INT_P 4
#define SIZEOF_UNSIGNED_CHAR 1
#define SIZEOF_UNSIGNED_SHORT 2
#define SIZEOF_UNSIGNED_INT 4
#define SIZEOF_UNSIGNED_LONG 4
#define SIZEOF_UNSIGNED_LONG_LONG 8

#if C_ATTRIBUTE_ALWAYS_INLINE
#define INLINE inline __attribute__((always_inline))
#else
#define INLINE inline
#endif

#if C_ATTRIBUTE_FASTCALL
#define DB_FASTCALL __attribute__((fastcall))
#else
#define DB_FASTCALL
#endif

#if C_HAS_ATTRIBUTE
#define GCC_ATTRIBUTE(x) __attribute__((x))
#else
#define GCC_ATTRIBUTE(x)
#endif

#if C_HAS_BUILTIN_EXPECT
#define GCC_UNLIKELY(x) __builtin_expect((x), 0)
#define GCC_LIKELY(x) __builtin_expect((x), 1)
#else
#define GCC_UNLIKELY(x) (x)
#define GCC_LIKELY(x) (x)
#endif

typedef double Real64;
typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned int Bit32u;
typedef signed int Bit32s;
typedef unsigned long long Bit64u;
typedef signed long long Bit64s;
typedef Bit32u Bitu;
typedef Bit32s Bits;

#endif
