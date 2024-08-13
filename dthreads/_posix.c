// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: _posix.c
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

#include "_headers/common.h"
#include "dthread.h"

int dthread_create(DThread* thread, DThreadAttr* attr, DThreadConfig* config)
{
    dthread_debug("dthread_create");

    assert(config && "Config cannot be NULL in dthread_create");

    pthread_attr_t p_attr;

    if (attr)
    {
        if (pthread_attr_init(&p_attr))
            return 1;

        if (attr->detachstate)
            pthread_attr_setdetachstate(&p_attr, attr->detachstate);

        if (attr->guardsize)
            pthread_attr_setguardsize(&p_attr, attr->guardsize);

#ifdef DTHREAD_THREAD_INHERITSCHED_AVAILABLE
        if (attr->inheritsched)
            pthread_attr_setinheritsched(&p_attr, attr->inheritsched);
#endif

        if (attr->schedpolicy)
            pthread_attr_setschedpolicy(&p_attr, attr->schedpolicy);

        if (attr->scope)
            pthread_attr_setscope(&p_attr, attr->scope);

#ifdef DTHREAD_THREAD_STACK_AVAILABLE
        if (attr->stack)
            pthread_attr_setstack(&p_attr, attr->stackaddr, attr->stack);
#endif

        if (attr->stacksize)
            pthread_attr_setstacksize(&p_attr, attr->stacksize);
    }

    return pthread_create(&thread->handle, attr ? &p_attr : NULL, config->func, &config->args);
}

int dthread_detach(DThread thread)
{
    dthread_debug("dthread_detach");

    return pthread_detach(thread.handle);
}

int dthread_join(DThread thread, void* code)
{
    dthread_debug("dthread_join");

    return pthread_join(thread.handle, code ? &code : NULL);
}

int dthread_equal(DThread thread1, DThread thread2)
{
    dthread_debug("dthread_equal");

    return pthread_equal(thread1.handle, thread2.handle);
}

DThread dthread_self(void)
{
    dthread_debug("dthread_self");

    DThread thread;

    thread.handle = pthread_self();

    return thread;
}

unsigned long dthread_id(DThread thread)
{
    dthread_debug("dthread_id");

    return (unsigned long)thread.handle;
}

void dthread_exit(void* code)
{
    dthread_debug("dthread_exit");

    pthread_exit(code);
}

int dthread_cancel(DThread thread)
{
    dthread_debug("dthread_cancel");

    return pthread_cancel(thread.handle);
}

int dthread_mutex_init(DThreadMutex* mutex, DThreadMutexAttr* attr)
{
    dthread_debug("dthread_mutex_init");

    pthread_mutexattr_t p_attr;

    if (attr)
    {
        if (pthread_mutexattr_init(&p_attr))
            return 1;
        if (attr->pshared)
            pthread_mutexattr_setpshared(&p_attr, attr->pshared);
#ifdef DTHREAD_MUTEX_TYPE_AVAILABLE
        if (attr->type)
            pthread_mutexattr_settype(&p_attr, attr->type);
#endif

#ifdef DTHREAD_MUTEX_ROBUST_AVAILABLE
        if (attr->robust)
            pthread_mutexattr_setrobust(&p_attr, attr->robust);
#endif

#ifdef DTHREAD_MUTEX_PROTOCOL_AVAILABLE
        if (attr->protocol)
            pthread_mutexattr_setprotocol(&p_attr, attr->protocol);
#endif

#ifdef DTHREAD_MUTEX_PRIOCEILING_AVAILABLE
        if (attr->prioceiling)
            pthread_mutexattr_setprioceiling(&p_attr, attr->prioceiling);
#endif
    }

    return pthread_mutex_init(&mutex->handle, attr ? &p_attr : NULL);
}

int dthread_mutex_lock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_lock");

    return pthread_mutex_lock(&mutex->handle);
}

int dthread_mutex_trylock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_trylock");

    return pthread_mutex_trylock(&mutex->handle);
}

int dthread_mutex_unlock(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_unlock");

    return pthread_mutex_unlock(&mutex->handle);
}

int dthread_mutex_destroy(DThreadMutex* mutex)
{
    dthread_debug("dthread_mutex_destroy");

    return pthread_mutex_destroy(&mutex->handle);
}

int dthread_cond_init(DThreadCond* cond, DThreadCondAttr* attr)
{
    dthread_debug("dthread_cond_init");

    pthread_condattr_t p_attr;

    if (attr)
    {
        if (pthread_condattr_init(&p_attr) != 0)
            return 1;

        if (attr->pshared)
            pthread_condattr_setpshared(&p_attr, attr->pshared);

#ifdef DTHREAD_COND_CLOCK_AVAILABLE
        if (attr->clock)
            pthread_condattr_setclock(&p_attr, attr->clock);
#endif
    }

    return pthread_cond_init(&cond->handle, attr ? &p_attr : NULL);
}

int dthread_cond_signal(DThreadCond* cond)
{
    dthread_debug("dthread_cond_signal");

    return pthread_cond_signal(&cond->handle);
}

int dthread_cond_broadcast(DThreadCond* cond)
{
    dthread_debug("dthread_cond_broadcast");

    return pthread_cond_broadcast(&cond->handle);
}

int dthread_cond_destroy(DThreadCond* cond)
{
    dthread_debug("dthread_cond_destroy");

    return pthread_cond_destroy(&cond->handle);
}

int dthread_cond_wait(DThreadCond* cond, DThreadMutex* mutex)
{
    dthread_debug("dthread_cond_wait");

    return pthread_cond_wait(&cond->handle, &mutex->handle);
}

#ifdef DTHREAD_RWLOCK_AVAILABLE

int dthread_rwlock_init(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_init");

    return pthread_rwlock_init(&rwlock->handle, NULL);
}

int dthread_rwlock_rdlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_rdlock");

    return pthread_rwlock_rdlock(&rwlock->handle);
}

int dthread_rwlock_unlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_unlock");

    return pthread_rwlock_unlock(&rwlock->handle);
}

int dthread_rwlock_wrlock(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_wrlock");

    return pthread_rwlock_wrlock(&rwlock->handle);
}

int dthread_rwlock_destroy(DThreadRWLock* rwlock)
{
    dthread_debug("dthread_rwlock_destroy");

    return pthread_rwlock_destroy(&rwlock->handle);
}

#endif