// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: dthread.h
//    Date: 2024-08-12
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description:
// ***************************************************************************************

#ifndef DTHREAD_H_

#define DTHREAD_H_

#if defined(_WIN32) || defined(_WIN64)

#include "_headers/windows.h"

#else

#include "_headers/posix.h"

#endif

typedef void* (*DThreadRoutine)(void*);

#define dthread_define_routine(NAME) void* NAME(void* arg)

#ifdef DTHREAD_DEBUG
#define dthread_debug(X) puts(X)
#else
#define dthread_debug(X)
#endif

typedef struct
{
    DThreadRoutine func;
    void* args;
} DThreadConfig;

#define dthread_config_init(FUNC, ARGS) \
    (DThreadConfig)                     \
    {                                   \
        .func = FUNC,                   \
        .args = (void*)(ARGS),          \
    }

int dthread_create(DThread* thread, DThreadAttr* attr, DThreadConfig* config);

int dthread_detach(DThread thread);

int dthread_join(DThread thread, void* code);

int dthread_equal(DThread thread1, DThread thread2);

DThread dthread_self(void);

unsigned long dthread_id(DThread thread);

void dthread_exit(void* code);

int dthread_cancel(DThread thread);

int dthread_mutex_init(DThreadMutex* mutex, DThreadMutexAttr* attr);

int dthread_mutex_lock(DThreadMutex* mutex);

int dthread_mutex_trylock(DThreadMutex* mutex);

int dthread_mutex_unlock(DThreadMutex* mutex);

int dthread_mutex_destroy(DThreadMutex* mutex);

int dthread_cond_init(DThreadCond* cond, DThreadCondAttr* attr);

int dthread_cond_signal(DThreadCond* cond);

int dthread_cond_broadcast(DThreadCond* cond);

int dthread_cond_destroy(DThreadCond* cond);

int dthread_cond_wait(DThreadCond* cond, DThreadMutex* mutex);

#ifdef DTHREAD_RWLOCK_AVAILABLE

int dthread_rwlock_init(DThreadRWLock* rwlock);

int dthread_rwlock_rdlock(DThreadRWLock* rwlock);

int dthread_rwlock_unlock(DThreadRWLock* rwlock);

int dthread_rwlock_wrlock(DThreadRWLock* rwlock);

int dthread_rwlock_destroy(DThreadRWLock* rwlock);

#endif

#ifdef DTHREAD_BARRIER_AVAILABLE

void dthread_barrier_init(DThreadBarrier* barrier, int num_threads);
void dthread_barrier_wait(DThreadBarrier* barrier);
void dthread_barrier_destroy(DThreadBarrier* barrier);

#endif

#ifdef DTHREAD_IMPL
#if defined(_WIN32) || defined(_WIN64)

#include "_windows.c"

#else

#include "_posix.c"

#endif
#endif

#endif // DTHREAD_H_
