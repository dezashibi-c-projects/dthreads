// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: posix.h
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

#ifndef DTHREAD_POSIX_H_

#define DTHREAD_POSIX_H_

#include <pthread.h>
#include <stddef.h>

#define DTHREAD_THREAD_STACKADDR_AVAILABLE
#define DTHREAD_THREAD_DETACHSTATE_AVAILABLE
#define DTHREAD_THREAD_GUARDSIZE_AVAILABLE
#define DTHREAD_THREAD_SCHEDPOLICY_AVAILABLE
#define DTHREAD_THREAD_SCOPE_AVAILABLE
#define DTHREAD_MUTEX_ATTR_AVAILABLE
#define DTHREAD_MUTEX_PSHARED_AVAILABLE
#define DTHREAD_COND_ATTR_AVAILABLE
#define DTHREAD_COND_PSHARED_AVAILABLE

#ifndef __ANDROID__
#define DTHREAD_THREAD_INHERITSCHED_AVAILABLE
#define DTHREAD_MUTEX_PROTOCOL_AVAILABLE
#define DTHREAD_MUTEX_PRIOCEILING_AVAILABLE
#endif

#if _POSIX_C_SOURCE >= 200112L
#define DTHREAD_THREAD_STACK_AVAILABLE
#endif

#if _POSIX_C_SOURCE >= 200809L
#define DTHREAD_MUTEX_TYPE_AVAILABLE
#endif

#if _POSIX_C_SOURCE >= 200112L

#define DTHREAD_RWLOCK_AVAILABLE
#define DTHREAD_COND_CLOCK_AVAILABLE

#if (defined(__linux__) || defined(__FreeBSD__)) && !defined __ANDROID__
#define DTHREAD_MUTEX_ROBUST_AVAILABLE
#endif

#endif

typedef struct DThread
{
    pthread_t handle;
} DThread;

typedef struct DThreadAttr
{
    size_t stacksize;
    void* stackaddr;
    int detachstate;
    size_t guardsize;
    int inheritsched;
    int schedpolicy;
    int scope;

#ifdef DTHREAD_THREAD_STACK_AVAILABLE
    size_t stack;
#endif
} DThreadAttr;

typedef struct DThreadMutex
{
    pthread_mutex_t handle;
} DThreadMutex;

typedef struct DThreadMutexAttr
{
    int pshared;

#ifdef DTHREAD_MUTEX_TYPE_AVAILABLE
    int type;
#endif

#ifdef DTHREAD_MUTEX_ROBUST_AVAILABLE
    int robust;
#endif

#ifdef DTHREAD_MUTEX_PROTOCOL_AVAILABLE
    int protocol;
#endif

#ifdef DTHREAD_MUTEX_PRIOCEILING_AVAILABLE
    int prioceiling;
#endif
} DThreadMutexAttr;

typedef struct DThreadCond
{
    pthread_cond_t handle;
} DThreadCond;

typedef struct DThreadCondAttr
{
    int pshared;

#ifdef DTHREAD_COND_CLOCK_AVAILABLE
    int clock;
#endif
} DThreadCondAttr;

#ifdef DTHREAD_RWLOCK_AVAILABLE
typedef struct DThreadRWLock
{
    pthread_rwlock_t handle;
} DThreadRWLock;
#endif

#endif // DTHREAD_POSIX_H_
