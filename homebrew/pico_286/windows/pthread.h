#ifndef R36SX_WINDOWS_PTHREAD_H
#define R36SX_WINDOWS_PTHREAD_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <process.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    INIT_ONCE once;
    CRITICAL_SECTION cs;
} pthread_mutex_t;

typedef CONDITION_VARIABLE pthread_cond_t;
typedef HANDLE pthread_t;

#define PTHREAD_MUTEX_INITIALIZER { INIT_ONCE_STATIC_INIT, { 0 } }
#define PTHREAD_COND_INITIALIZER CONDITION_VARIABLE_INIT

static BOOL CALLBACK r36sx_windows_pthread_mutex_init_once(
    PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    (void)once;
    (void)context;
    InitializeCriticalSection(&((pthread_mutex_t *)parameter)->cs);
    return TRUE;
}

static int r36sx_windows_pthread_mutex_ready(pthread_mutex_t *mutex)
{
    return InitOnceExecuteOnce(&mutex->once,
                               r36sx_windows_pthread_mutex_init_once,
                               mutex, NULL) ? 0 : -1;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if (r36sx_windows_pthread_mutex_ready(mutex) != 0) {
        return -1;
    }
    EnterCriticalSection(&mutex->cs);
    return 0;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    LeaveCriticalSection(&mutex->cs);
    return 0;
}

static int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    if (r36sx_windows_pthread_mutex_ready(mutex) != 0) {
        return -1;
    }
    return SleepConditionVariableCS(cond, &mutex->cs, INFINITE) ? 0 : -1;
}

static int pthread_cond_signal(pthread_cond_t *cond)
{
    WakeConditionVariable(cond);
    return 0;
}

static int pthread_cond_broadcast(pthread_cond_t *cond)
{
    WakeAllConditionVariable(cond);
    return 0;
}

struct r36sx_windows_thread_start {
    void *(*start)(void *);
    void *arg;
};

static unsigned __stdcall r36sx_windows_pthread_trampoline(void *opaque)
{
    struct r36sx_windows_thread_start *ctx =
        (struct r36sx_windows_thread_start *)opaque;
    void *(*start)(void *) = ctx->start;
    void *arg = ctx->arg;
    free(ctx);
    (void)start(arg);
    return 0;
}

static int pthread_create(pthread_t *thread, const void *attr,
                          void *(*start)(void *), void *arg)
{
    struct r36sx_windows_thread_start *ctx;
    uintptr_t handle;
    (void)attr;

    ctx = (struct r36sx_windows_thread_start *)calloc(1, sizeof(*ctx));
    if (!ctx) {
        return -1;
    }
    ctx->start = start;
    ctx->arg = arg;

    handle = _beginthreadex(NULL, 0, r36sx_windows_pthread_trampoline,
                            ctx, 0, NULL);
    if (handle == 0) {
        free(ctx);
        return -1;
    }
    *thread = (HANDLE)handle;
    return 0;
}

static int pthread_cancel(pthread_t thread)
{
    /*
     * The Windows debug host only uses pthread_cancel during process shutdown.
     * TerminateThread is deliberately kept out of the device path and avoids
     * hanging the debugger if the audio thread is blocked in a condition wait.
     */
    return TerminateThread(thread, 0) ? 0 : -1;
}

static int pthread_join(pthread_t thread, void **retval)
{
    (void)retval;
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
