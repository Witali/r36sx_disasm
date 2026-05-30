/*
 * R36SX entry wrapper for DOSBox.
 *
 * The upstream SDL frontend still owns the DOSBox runtime.  This wrapper only
 * prepares the firmware environment and injects the local dosbox.conf path so
 * TinyMC or a shell can launch the binary directly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int dosbox_main(int argc, char *argv[]);

static const char *kAppDir = "/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx";
static const char *kLibraryPath =
    "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:"
    "/mnt/sdcard/cubegm/usr/lib32:/lib:/usr/lib";

static int has_conf_arg(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-conf") == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    setenv("LD_LIBRARY_PATH", kLibraryPath, 1);
    setenv("SDL_NOMOUSE", "1", 0);

    FILE *probe = fopen("/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx/dosbox.conf", "r");
    if (probe) {
        fclose(probe);
        chdir(kAppDir);
    }

    freopen("/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx/dosbox.stdout.log", "w", stdout);
    freopen("/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx/dosbox.stderr.log", "w", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    if (has_conf_arg(argc, argv)) {
        return dosbox_main(argc, argv);
    }

    char **wrapped_argv = (char **)calloc((size_t)argc + 3u, sizeof(char *));
    if (!wrapped_argv) {
        return dosbox_main(argc, argv);
    }

    for (int i = 0; i < argc; i++) {
        wrapped_argv[i] = argv[i];
    }
    wrapped_argv[argc] = (char *)"-conf";
    wrapped_argv[argc + 1] = (char *)"dosbox.conf";
    wrapped_argv[argc + 2] = NULL;

    int rc = dosbox_main(argc + 2, wrapped_argv);
    free(wrapped_argv);
    return rc;
}
