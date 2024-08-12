// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c-projects/dthreads
//    File: windows.h
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

#ifndef DTHREAD_WINDOWS_H_

#define DTHREAD_WINDOWS_H_

#include <stddef.h>
#include <windows.h>

#define DTHREAD_RWLOCK_AVAILABLE
#define DTHREAD_CREATION_FLAG_AVAILABLE

typedef struct DThread
{
    HANDLE handle;
} DThread;

typedef struct DThreadAttr
{
    size_t stacksize;
    int dwCreationFlags;
} DThreadAttr;

typedef struct DThreadMutex
{
    CRITICAL_SECTION handle;
} DThreadMutex;

typedef struct DThreadMutexAttr
{
    void* nothing;
} DThreadMutexAttr;

typedef struct DThreadCond
{
    CONDITION_VARIABLE handle;
} DThreadCond;

typedef struct DThreadCondAttr
{
    void* nothing;
} DThreadCondAttr;

typedef struct DThreadRWLock
{
    int type;
    PSRWLOCK handle;
} DThreadRWLock;

#endif // DTHREAD_WINDOWS_H_
