#ifndef R36SX_WINDOWS_DLFCN_H
#define R36SX_WINDOWS_DLFCN_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define RTLD_NOW 0

static inline void *dlopen(const char *filename, int flags)
{
    (void)flags;
    return filename ? (void *)LoadLibraryA(filename) : NULL;
}

static inline void *dlsym(void *handle, const char *symbol)
{
    return handle && symbol ? (void *)GetProcAddress((HMODULE)handle, symbol)
                            : NULL;
}

static inline int dlclose(void *handle)
{
    return handle ? (FreeLibrary((HMODULE)handle) ? 0 : -1) : 0;
}

#endif
