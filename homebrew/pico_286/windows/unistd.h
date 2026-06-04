#ifndef R36SX_WINDOWS_UNISTD_H
#define R36SX_WINDOWS_UNISTD_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <direct.h>
#include <io.h>
#include <stdlib.h>
#include <windows.h>

#define getcwd _getcwd
#define access _access
#ifndef R_OK
#define R_OK 4
#endif

static int usleep(unsigned int usec)
{
    DWORD msec = (usec + 999u) / 1000u;
    Sleep(msec ? msec : 1u);
    return 0;
}

static int setenv(const char *name, const char *value, int overwrite)
{
    if (!overwrite) {
        char *existing = NULL;
        size_t existing_size = 0;
        if (_dupenv_s(&existing, &existing_size, name) == 0 && existing) {
            free(existing);
            return 0;
        }
    }
    return _putenv_s(name, value ? value : "");
}

#endif
