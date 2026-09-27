#ifndef R36SX_WINDOWS_DIRENT_H
#define R36SX_WINDOWS_DIRENT_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* The disk menu only needs names. Keep one Win32 search handle per DIR and
 * close it even when the caller stops scanning before the last entry. */
struct dirent {
    char d_name[MAX_PATH];
};

typedef struct {
    HANDLE handle;
    WIN32_FIND_DATAA data;
    struct dirent entry;
    int first;
} DIR;

static inline DIR *opendir(const char *path)
{
    char pattern[MAX_PATH];
    DWORD attributes;
    size_t length = strlen(path);
    DIR *dir;
    if (length + 3 > sizeof(pattern)) {
        errno = ENAMETOOLONG;
        return NULL;
    }
    attributes = GetFileAttributesA(path);
    if (attributes == INVALID_FILE_ATTRIBUTES ||
        !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        errno = ENOENT;
        return NULL;
    }
    dir = (DIR *)calloc(1, sizeof(*dir));
    if (!dir) {
        errno = ENOMEM;
        return NULL;
    }
    memcpy(pattern, path, length);
    if (length && path[length - 1] != '/' && path[length - 1] != '\\') {
        pattern[length++] = '\\';
    }
    pattern[length++] = '*';
    pattern[length] = '\0';
    dir->handle = FindFirstFileA(pattern, &dir->data);
    dir->first = 1;
    if (dir->handle == INVALID_HANDLE_VALUE && GetLastError() != ERROR_FILE_NOT_FOUND) {
        free(dir);
        errno = EACCES;
        return NULL;
    }
    return dir;
}

static inline struct dirent *readdir(DIR *dir)
{
    if (dir->handle == INVALID_HANDLE_VALUE ||
        (!dir->first && !FindNextFileA(dir->handle, &dir->data))) {
        return NULL;
    }
    dir->first = 0;
    memcpy(dir->entry.d_name, dir->data.cFileName, sizeof(dir->entry.d_name));
    return &dir->entry;
}

static inline int closedir(DIR *dir)
{
    int result = 0;
    if (dir->handle != INVALID_HANDLE_VALUE && !FindClose(dir->handle)) {
        result = -1;
    }
    free(dir);
    return result;
}

#endif
