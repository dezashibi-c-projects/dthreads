# 👉👉 NOTE

## This is not fully completed, I'm working on it, and am open to hear what you think

Here are a couple of areas that I would like to get help or opinions:

- [ ] Random number generator behavior improvement, especially on Windows refer to [here](https://github.com/dezashibi-c/dthreads/blob/0fd1c7339f26b0d6409b6ae8ac03d981cd795c4a/dthreads/_headers/random.h#L82).
- [ ] haven't tested it with `cpp` yet
- [ ] Can I consider it feature completed? any suggestions?
- [x] Comments and documentations are not yet fully reviewed
- [x] API declarations need review (for more cross-compiler support and also dynamic linking)

# DThreads: Cross-Platform Threading Library in C

After using [CThreads](https://github.com/PerformanC/CThreads) by [@ThePedroo](https://github.com/ThePedroo) at [a specific commit](https://github.com/PerformanC/CThreads/commit/805673c9161f62e542ada76080d59516da5d17fd) in the repository (since there were no tags or version numbers), I found the need to adapt and re-implement it to fit [my coding style](https://github.com/dezashibi-c/.github/blob/main/coding_style.md) and preferences to have a simpler, more compatible with Windows API (the result from thread had bug) and more feature-rich version.

This library is based on the ideas from `CThreads` as the initial inspiration with a significant re-implementation with additional functionality and design choices.

**👉 NOTE:** In accordance with the original BSD 2-Clause License of `CThreads`, I acknowledge the work of [@ThePedroo](https://github.com/ThePedroo). This project includes substantial re-implementations, modifications and additions, and is distributed under the BSD 3-Clause License. The original `CThreads` license is included in this project, ensuring compliance with open-source licensing requirements.

## Overview

DThreads is a cross-platform threading library designed for C programmers who need a consistent and simple API to manage threads, synchronization primitives, and other concurrency utilities across different operating systems. It abstracts the complexities of native threading APIs like POSIX threads and Windows threads, providing a unified interface that works seamlessly on both platforms.

## Design Philosophy

The core philosophy behind DThreads is to create a minimalistic, yet powerful, threading library that emphasizes portability, simplicity, and performance. By abstracting the differences between POSIX and Windows threading mechanisms, DThreads allows programmers to write multithreaded code once and run it on multiple platforms without modification.

**👉 NOTE:** When implementing `dthreads` library, despite trying to keep the API closer to `pthread` but my assumption was the programmer wants single source of truth so the documentation tried to explain things more in detail rather than referring to original API documentations for `pthread` and Windows API.

### Key Design Choices

- **Cross-Platform Compatibility**: DThreads provides a unified API that works across different platforms, hiding the underlying implementation details.
- **Modularity**: I've not been a friend of everything all at one place, so I've decided to modularize the implementations for each platform to its corresponding header and source file.
- **Simplicity**: The API is designed to be easy to use, with clear and concise function signatures and configuration structures.
- **Performance**: DThreads aims to minimize overhead by using lightweight structures and efficient synchronization primitives.
- **Code Clarity**: The library encourages a clear and consistent coding style, with well-documented functions and macros to reduce the learning curve.

## API Overview

**👉 For detailed documentation refer to [dthread.h](/dthreads/dthread.h).**

### Thread Management

- **DThreadRoutine**: A function pointer type for thread routines. A thread routine is a function that accepts a single `void*` argument and returns a `void*`.
- **dthread_define_routine**: A macro to simplify the definition of thread routines. It ensures compatibility with the DThreads library.
- **dthread_set_data**: A macro that sets thread data for given thread reference.
- **dthread_set_func**: A macro that sets thread routine for given thread reference.
- **dthread_get_result**: A macro that gets thread routine result after the thread join completes for given thread reference.
- **dthread_get_result_as**: A macro that gets thread routine result after the thread join completes for given thread reference and casting it to given type (must be a pointer type).
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

### Thread Safe Random Number Generator

- **dthread_rng_init**: Initializes the mutex for thread-safe random number generation.
- **dthread_rng_cleanup**: Cleans up the mutex used for thread-safe random number generation.
- **dthread_rng_random**: Generates a thread-safe random number.
- **dthread_rng_seed_maker**: Seeds the random number generator with a unique seed.

**👉 NOTE: Checkout [trylock.c](/examples/trylock.c) for learning more about using thread safe random number generator.**

### Types Documentation

**👉 NOTE:** Types are defined in [dthread.h](/dthreads/dthread.h) and in the library's [windows.h](/dthreads/_headers/windows.h) and [posix.h](/dthreads/_headers/posix.h) based on the operating system accordingly. You can find the overall definition and purpose of each type below.

- **`DThread`**  
  Represents a thread in the DThreads library.  
  The `DThread` structure is used to manage and identify individual threads created and managed by the DThreads library. It abstracts the underlying platform-specific thread representation. It also holds the reference to thread routine, thread routine data and thread result.

- **`DThreadAttr`**  
  Attributes for thread creation.  
  The `DThreadAttr` structure is used to specify attributes for threads when they are created. This includes options like stack size, thread priority, and other platform-specific attributes that influence the behavior of the thread.

- **`DThreadMutex`**  
  Represents a mutex (mutual exclusion) in the DThreads library.  
  The `DThreadMutex` structure is used to protect shared resources from concurrent access by multiple threads. It provides locking mechanisms to ensure that only one thread can access a critical section at a time.

- **`DThreadMutexAttr`**  
  Attributes for mutex creation.  
  The `DThreadMutexAttr` structure is used to specify attributes for mutexes when they are initialized. This includes options like mutex type (normal, recursive, or error-checking) and other platform-specific attributes.

- **`DThreadCond`**  
  Represents a condition variable in the DThreads library.  
  The `DThreadCond` structure is used for thread synchronization by allowing threads to wait until a particular condition is met. Condition variables are used in conjunction with mutexes to avoid race conditions.

- **`DThreadCondAttr`**  
  Attributes for condition variable creation.  
  The `DThreadCondAttr` structure is used to specify attributes for condition variables when they are initialized. These attributes may vary depending on the underlying platform.

- **`DThreadRWLock`**  
  Represents a read-write lock in the DThreads library.  
  The `DThreadRWLock` structure is used to provide read-write synchronization. It allows multiple threads to read shared data simultaneously, while ensuring exclusive access for write operations.

- **`DThreadBarrier`**  
  Represents a barrier in the DThreads library.  
  The `DThreadBarrier` structure is used to synchronize a group of threads at a specific point in the program. All threads must reach the barrier before any can proceed past it.

- **`DThreadSemaphore`**  
  Represents a semaphore in the DThreads library.  
  The `DThreadSemaphore` structure is used to control access to a resource by multiple threads. Semaphores are used to limit the number of threads that can access a resource concurrently.

### **Constant Macro Documentation**

#### **`DTHREAD_RWLOCK_AVAILABLE`**

This macro indicates the availability of read-write locks (RWLocks) within the DThreads library. If this macro is defined, it means that the platform supports RWLocks, and the corresponding APIs (`dthread_rwlock_init`, `dthread_rwlock_rdlock`, etc.) are available for use. On platforms where RWLocks are not supported, this macro is undefined, and the RWLock-related APIs will not be available.

#### **`DTHREAD_CREATION_FLAG_AVAILABLE`**

This macro signifies that thread creation flags are available on the platform. These flags allow additional control over thread behavior at the time of creation, such as setting the thread to be created in a suspended state. If this macro is defined, the `DThreadAttr` structure includes a `dwCreationFlags` member for Windows or an equivalent in POSIX systems.

#### **`DTHREAD_BARRIER_AVAILABLE`**

This macro indicates whether barrier synchronization primitives are supported on the platform. A barrier is a point where multiple threads must wait until all have reached the barrier before any can proceed. If this macro is defined, the barrier-related functions (`dthread_barrier_init`, `dthread_barrier_wait`, etc.) are available. If not defined, barrier synchronization is not supported on the platform, and these functions will not be present.

#### **`DTHREAD_SEMAPHORE_AVAILABLE`**

This macro indicates the availability of semaphores within the DThreads library. Semaphores are synchronization primitives used to control access to a common resource by multiple threads. If this macro is defined, semaphore-related functions (`dthread_semaphore_init`, `dthread_semaphore_wait`, etc.) are available for use. In the absence of this macro, semaphore support is not provided by the platform, and these functions will be excluded.

#### **`DTHREAD_MUTEX_TYPE_AVAILABLE`**

This macro is used to indicate whether the platform supports different types of mutexes. In POSIX systems, this corresponds to the availability of `pthread_mutexattr_settype`. The `DThreadMutexAttr` structure will include a `type` member to specify the mutex type, such as normal, recursive, or error-checking. If this macro is not defined, only the default mutex type is supported.

#### **`DTHREAD_MUTEX_ROBUST_AVAILABLE`**

This macro indicates the availability of robust mutexes, which are designed to handle cases where a thread holding a mutex is terminated without releasing it. If defined, the `DThreadMutexAttr` structure will include a `robust` member, and functions like `dthread_mutex_init` will support robust mutex attributes. In the absence of this macro, robust mutexes are not supported.

#### **`DTHREAD_MUTEX_PROTOCOL_AVAILABLE`**

This macro denotes the availability of mutex protocol attributes, which control the priority inversion handling mechanism. If defined, the `DThreadMutexAttr` structure includes a `protocol` member to specify the mutex protocol. This feature is primarily available on real-time operating systems and is not commonly supported on all platforms.

#### **`DTHREAD_MUTEX_PRIOCEILING_AVAILABLE`**

This macro indicates that the platform supports the priority ceiling attribute for mutexes. If defined, the `DThreadMutexAttr` structure will include a `prioceiling` member, allowing you to set the priority ceiling for mutexes to prevent priority inversion. If not defined, priority ceiling is not supported by the platform.

#### **`DTHREAD_DEBUG`**

This macro is used to control the logging of debug information within the DThreads library. When defined, it enables the `dthread_debug` and `dthread_debug_args` function macros, which logs internal operations and state changes. This is useful for development and troubleshooting but should be disabled in production builds to avoid performance overhead.

## Usage Guide

**👉 NOTE: You can get the latest `rc`, `beta`, or stable version from releases. You can use the following commands:**

- the latest release:

```bash
 wget https://github.com/dezashibi-c/dthreads/releases/download/$(curl -s https://api.github.com/repos/dezashibi-c/dthreads/releases/latest | grep -oP '"tag_name": "\K(.*)(?=")')/dthreads.zip
```

- the specific version (remember to change `<version>` with the version you need)

```bash
https://github.com/dezashibi-c/dthreads/releases/download/<version>/dthreads.zip
```

### Including DThreads in Your Project

**👉 NOTE: Make sure `#define DTHREAD_IMPL` is included in exactly one of your source files.**

**👉 NOTE: Make sure to check out the [examples folder](/examples/) for usage examples.**

**👉 NOTE: Running `make` on the root of the cloned repo builds all the [examples folder](/examples/).**

**👉 NOTE: You can run all the tests all at once by running `make test`.**

**👉 NOTE: Don't forget to add `-lpthread` when compiling on POSIX operating systems.**

To include DThreads in your project, add the `dthreads/dthread.h` header file to your source files and link against the appropriate implementation for your platform (POSIX or Windows).

```c
#define DTHREAD_IMPL
#include "dthreads/dthread.h"
```

### Creating a Thread

To create a thread, define a thread routine and use `dthread_create` to start the thread:

```c
#include "dthreads/dthread.h"

dthread_define_routine(my_thread_function) {
    // Thread code here
    return NULL;
}

int main() {
    DThread thread = dthread_new_config(my_thread_function, NULL);

    dthread_create(&thread, NULL);
    dthread_join(&thread);
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

### Debugging Macro

You can add whether `#define DTHREAD_DEBUG` before including the header file or passing `-DDTHREAD_DEBUG` to your compiler to activate debug messages.

**👉 NOTE: Checkout [basic.c](/examples/basic.c) example to learn more.**

- Output for `basic.c`

```powershell
>.\examples\basic.exe

----- value started at 12000
dthread_mutex_init
dthread_create
dthread_create
dthread_mutex_lock
dthread_create
dthread_mutex_lock
dthread_mutex_lock
dthread_create
----- value is now 12001
dthread_join
dthread_mutex_lock
dthread_mutex_unlock
----- value is now 12002
dthread_mutex_unlock
dthread_join
dthread_join
----- value is now 12003
dthread_mutex_unlock
----- value is now 12004
dthread_mutex_unlock
dthread_join
dthread_mutex_destroy
----- value finished with 12004
final result: 12004
```

### How to Use the `dthreads` Library in Shared Libraries

The `dthreads` library is designed to be used both as a static library and as a dynamic/shared library.

#### 1. **Understanding the `DTHREAD_API` Macro**

The `dthreads` library uses a macro called `DTHREAD_API` to manage the export and import of symbols when building and using shared libraries. This macro adapts to different compilers and platforms to ensure that functions are correctly exported from the DLL (or shared object) and imported by any application or library that uses `dthreads`.

- **`DTHREAD_API_EXPORT`**: Used when building the `dthreads` library to export functions and variables.
- **`DTHREAD_API_IMPORT`**: Used when including `dthreads` in another project to import functions and variables from the shared library.
- **`DTHREAD_DLL_EXPORTS`**: This macro should be defined when building the shared library to enable the export of symbols.

#### 2. **Building the `dthreads` Library as a Shared Library**

When compiling the `dthreads` library as a shared library, ensure that the `DTHREAD_DLL_EXPORTS` macro is defined. This will cause the `DTHREAD_API` macro to expand to `DTHREAD_API_EXPORT`, ensuring that all public functions and variables are exported from the DLL.

Example command for building with MSVC:

```sh
cl /D DTHREAD_DLL_EXPORTS /LD dthread.c /o dthread.dll
```

Example command for building with GCC:

```sh
gcc -DDTHREAD_DLL_EXPORTS -shared -o libdthread.so dthread.c
```

#### 3. **Using the `dthreads` Library in Your Application**

When using the `dthreads` library in your application or another shared library, you do **not** need to define `DTHREAD_DLL_EXPORTS`. This allows the `DTHREAD_API` macro to expand to `DTHREAD_API_IMPORT`, which ensures that the symbols are imported from the DLL.

**👉 NOTE:** You only need this if you're creating another library based on `dthreads` and you need to choose to link it to your program statically or dynamically, otherwise just follow the normal way and add `DTHREAD_IMPL` in one of your `.c` source files and that does the work.

## Contribution

Please consider **discussion in the issue section** for adding features or changing behaviors beforehand and please refer to [my coding style guide](https://github.com/dezashibi-c/.github/blob/main/coding_style.md) if you'd like to contribute.

## Attributions

Thanks to [code-vault](https://code-vault.net) for providing various practical examples on their website so that I could test out my library properly.

## License

This project is licensed under the BSD 3-Clause License.

Some parts of this project are derived from the `CThreads` library by [@ThePedroo](https://github.com/ThePedroo), which is licensed under the BSD 2-Clause License. To comply with the terms of the BSD 2-Clause License, a copy of the original `CThreads` license is included in this repository under the [LICENSE](/LICENSE) file.
