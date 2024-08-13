# DThreads: Cross-Platform Threading Library in C

## Overview

DThreads is a cross-platform threading library designed for C programmers who need a consistent and simple API to manage threads, synchronization primitives, and other concurrency utilities across different operating systems. It abstracts the complexities of native threading APIs like POSIX threads and Windows threads, providing a unified interface that works seamlessly on both platforms.

**👉 NOTE:** I got this idea by looking at a library called [CThreads](https://github.com/PerformanC/CThreads) by [@ThePedroo](https://github.com/ThePedroo). I have used it a couple of tiles when it was at [this commit](https://github.com/PerformanC/CThreads/commit/805673c9161f62e542ada76080d59516da5d17fd) and I thought I want to sort of hacking it and making a newer and simpler one based on [my coding style and tastes](https://github.com/dezashibi-c/.github/blob/main/coding_style.md) and also add more features to it. So I can say mention here that this is in fact a hacky and re-imagined re-implementation of `CThreads` library.

## Design Philosophy

The core philosophy behind DThreads is to create a minimalistic, yet powerful, threading library that emphasizes portability, simplicity, and performance. By abstracting the differences between POSIX and Windows threading mechanisms, DThreads allows programmers to write multithreaded code once and run it on multiple platforms without modification.

**👉 NOTE:** When implementing `dthreads` library, despite trying to keep the API closer to `pthread` but my assumption was the programmer wants single source of truth so the documentation tried to explain things more in detail rather than referring to original API documentations for `pthread` and Windows API.

### Key Design Choices

1. **Cross-Platform Compatibility**: DThreads provides a unified API that works across different platforms, hiding the underlying implementation details.
2. **Modularity**: I've not been a friend of everything all at one place, so I've decided to modularize the implementations for each platform to its corresponding header and source file.
3. **Simplicity**: The API is designed to be easy to use, with clear and concise function signatures and configuration structures.
4. **Performance**: DThreads aims to minimize overhead by using lightweight structures and efficient synchronization primitives.
5. **Code Clarity**: The library encourages a clear and consistent coding style, with well-documented functions and macros to reduce the learning curve.

## API Overview

**👉 For detailed documentation refer to [dthread.h](/dthreads/dthread.h).**

### Thread Management

- **DThreadRoutine**: A function pointer type for thread routines. A thread routine is a function that accepts a single `void*` argument and returns a `void*`.
- **dthread_define_routine(NAME)**: A macro to simplify the definition of thread routines. It ensures compatibility with the DThreads library.
- **dthread_create**: Creates a new thread using the specified configuration and attributes.
- **dthread_detach**: Detaches a thread, allowing it to run independently. Once detached, a thread cannot be joined.
- **dthread_join**: Waits for a thread to complete, blocking the calling thread until the specified thread terminates.
- **dthread_equal**: Compares two threads for equality.
- **dthread_self**: Returns a `DThread` structure representing the current thread.
- **dthread_id**: Returns the unique identifier of a thread.
- **dthread_exit**: Exits the calling thread and optionally returns a value to the thread that joined it.
- **dthread_cancel**: Sends a cancellation request to the specified thread.

### Synchronization Primitives

- **Mutexes**:
  - **dthread_mutex_init**: Initializes a mutex with optional attributes.
  - **dthread_mutex_lock**: Locks a mutex, blocking the calling thread if necessary.
  - **dthread_mutex_trylock**: Attempts to lock a mutex without blocking.
  - **dthread_mutex_unlock**: Unlocks a mutex.
  - **dthread_mutex_destroy**: Destroys a mutex, releasing its resources.
  
- **Condition Variables**:
  - **dthread_cond_init**: Initializes a condition variable with optional attributes.
  - **dthread_cond_signal**: Signals a condition variable, waking one waiting thread.
  - **dthread_cond_broadcast**: Broadcasts a condition variable, waking all waiting threads.
  - **dthread_cond_destroy**: Destroys a condition variable, releasing its resources.
  - **dthread_cond_wait**: Waits on a condition variable, releasing the associated mutex and blocking the calling thread until the condition is signaled.

### Advanced Features (Optional)

- **Read-Write Locks** (if `DTHREAD_RWLOCK_AVAILABLE` is defined):
  - **dthread_rwlock_init**: Initializes a read-write lock.
  - **dthread_rwlock_rdlock**: Acquires a read lock on the read-write lock.
  - **dthread_rwlock_unlock**: Unlocks the read-write lock.
  - **dthread_rwlock_wrlock**: Acquires a write lock on the read-write lock.
  - **dthread_rwlock_destroy**: Destroys the read-write lock.
  
- **Barriers** (if `DTHREAD_BARRIER_AVAILABLE` is defined):
  - **dthread_barrier_init**: Initializes a barrier for a specified number of threads.
  - **dthread_barrier_wait**: Waits at a barrier until the specified number of threads have reached the barrier.
  - **dthread_barrier_destroy**: Destroys the barrier, releasing its resources.
  
- **Semaphores** (if `DTHREAD_SEMAPHORE_AVAILABLE` is defined):
  - **dthread_semaphore_init**: Initializes a semaphore with the specified initial value.
  - **dthread_semaphore_wait**: Waits on a semaphore, decrementing its value.
  - **dthread_semaphore_post**: Posts to a semaphore, incrementing its value.
  - **dthread_semaphore_destroy**: Destroys the semaphore, releasing its resources.

Certainly! Below is the documentation for all the macros used in the DThreads library. Each macro's purpose and usage are explained in detail to avoid any confusion.

### **Macro Documentation**

#### **1. `DTHREAD_RWLOCK_AVAILABLE`**

This macro indicates the availability of read-write locks (RWLocks) within the DThreads library. If this macro is defined, it means that the platform supports RWLocks, and the corresponding APIs (`dthread_rwlock_init`, `dthread_rwlock_rdlock`, etc.) are available for use. On platforms where RWLocks are not supported, this macro is undefined, and the RWLock-related APIs will not be available.

#### **2. `DTHREAD_CREATION_FLAG_AVAILABLE`**

This macro signifies that thread creation flags are available on the platform. These flags allow additional control over thread behavior at the time of creation, such as setting the thread to be created in a suspended state. If this macro is defined, the `DThreadAttr` structure includes a `dwCreationFlags` member for Windows or an equivalent in POSIX systems.

#### **3. `DTHREAD_BARRIER_AVAILABLE`**

This macro indicates whether barrier synchronization primitives are supported on the platform. A barrier is a point where multiple threads must wait until all have reached the barrier before any can proceed. If this macro is defined, the barrier-related functions (`dthread_barrier_init`, `dthread_barrier_wait`, etc.) are available. If not defined, barrier synchronization is not supported on the platform, and these functions will not be present.

#### **4. `DTHREAD_SEMAPHORE_AVAILABLE`**

This macro indicates the availability of semaphores within the DThreads library. Semaphores are synchronization primitives used to control access to a common resource by multiple threads. If this macro is defined, semaphore-related functions (`dthread_semaphore_init`, `dthread_semaphore_wait`, etc.) are available for use. In the absence of this macro, semaphore support is not provided by the platform, and these functions will be excluded.

#### **5. `DTHREAD_MUTEX_TYPE_AVAILABLE`**

This macro is used to indicate whether the platform supports different types of mutexes. In POSIX systems, this corresponds to the availability of `pthread_mutexattr_settype`. The `DThreadMutexAttr` structure will include a `type` member to specify the mutex type, such as normal, recursive, or error-checking. If this macro is not defined, only the default mutex type is supported.

#### **6. `DTHREAD_MUTEX_ROBUST_AVAILABLE`**

This macro indicates the availability of robust mutexes, which are designed to handle cases where a thread holding a mutex is terminated without releasing it. If defined, the `DThreadMutexAttr` structure will include a `robust` member, and functions like `dthread_mutex_init` will support robust mutex attributes. In the absence of this macro, robust mutexes are not supported.

#### **7. `DTHREAD_MUTEX_PROTOCOL_AVAILABLE`**

This macro denotes the availability of mutex protocol attributes, which control the priority inversion handling mechanism. If defined, the `DThreadMutexAttr` structure includes a `protocol` member to specify the mutex protocol. This feature is primarily available on real-time operating systems and is not commonly supported on all platforms.

#### **8. `DTHREAD_MUTEX_PRIOCEILING_AVAILABLE`**

This macro indicates that the platform supports the priority ceiling attribute for mutexes. If defined, the `DThreadMutexAttr` structure will include a `prioceiling` member, allowing you to set the priority ceiling for mutexes to prevent priority inversion. If not defined, priority ceiling is not supported by the platform.

#### **9. `DTHREAD_DEBUG`**

This macro is used to control the logging of debug information within the DThreads library. When defined, it enables the `dthread_debug` function or macro, which logs internal operations and state changes. This is useful for development and troubleshooting but should be disabled in production builds to avoid performance overhead.

## Usage Guide

### Including DThreads in Your Project

**👉 NOTE: Make sure `#define DTHREAD_IMPL` is included in exactly one of your source files.**

**👉 NOTE2: Make sure to check out [example folder](/examples/).**

To include DThreads in your project, add the `dthread.h` header file to your source files and link against the appropriate implementation for your platform (POSIX or Windows).

```c
#define DTHREAD_IMPL
#include "dthread.h"
```

### Creating a Thread

To create a thread, define a thread routine and use `dthread_create` to start the thread:

```c
#include "dthread.h"

dthread_define_routine(my_thread_function) {
    // Thread code here
    return NULL;
}

int main() {
    DThread thread;
    DThreadConfig config = dthread_config_init(my_thread_function, NULL);
    dthread_create(&thread, NULL, &config);
    dthread_join(thread, NULL);
    return 0;
}
```

### Using Mutexes

Mutexes are used to protect shared resources from concurrent access:

```c
DThreadMutex mutex;
dthread_mutex_init(&mutex, NULL);

dthread_mutex_lock(&mutex);
// Critical section
dthread_mutex_unlock(&mutex);

dthread_mutex_destroy(&mutex);
```

### Condition Variables

Condition variables are used to block a thread until a particular condition is met:

```c
DThreadCond cond;
DThreadMutex mutex;

dthread_cond_init(&cond, NULL);
dthread_mutex_init(&mutex, NULL);

dthread_mutex_lock(&mutex);
while (condition_not_met()) {
    dthread_cond_wait(&cond, &mutex);
}
dthread_mutex_unlock(&mutex);

dthread_cond_signal(&cond);
dthread_cond_destroy(&cond);
dthread_mutex_destroy(&mutex);
```

### Using Semaphores

Semaphores control access to a resource by multiple threads:

```c
DThreadSemaphore semaphore;
dthread_semaphore_init(&semaphore, 3); // Initialize with a count of 3

dthread_semaphore_wait(&semaphore);
// Access the resource
dthread_semaphore_post(&semaphore);

dthread_semaphore_destroy(&semaphore);
```

### Barriers

Barriers are used to synchronize a group of threads:

```c
DThreadBarrier barrier;
dthread_barrier_init(&barrier, 5); // Barrier for 5 threads

dthread_barrier_wait(&barrier);
// Code that executes after all threads reach the barrier

dthread_barrier_destroy(&barrier);
```

## Contribution

Please refer to [my coding style guide](https://github.com/dezashibi-c/.github/blob/main/coding_style.md) if you'd like to contribute.

## License for `dthreads` library

BSD 3-Clause License

Please refer to [LICENSE](/LICENSE) file.
