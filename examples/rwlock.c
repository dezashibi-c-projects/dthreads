
#define DTHREAD_DEBUG
#define DTHREAD_IMPL
#include "../dthreads/dthread.h"
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define xsleep(x) Sleep((x))
#else
void xsleep(dthread_uint_t milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
#endif

#define NUM_READERS 5
#define NUM_WRITERS 2
#define NUM_ITERATIONS 10

int shared_data = 0;
DThreadRWLock rwlock;

dthread_define_routine(reader)
{
    (void)data;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        // Acquire read lock
        dthread_rwlock_rdlock(&rwlock);

        // Read the shared data
        printf("Reader %zu read shared_data = %d\n", (dthread_ullong_t)dthread_self(), shared_data);

        // Release read lock
        dthread_rwlock_unlock(&rwlock);

        // Simulate some work
        xsleep(1000);
    }
    return NULL;
}

dthread_define_routine(writer)
{
    (void)data;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        // Acquire write lock
        dthread_rwlock_wrlock(&rwlock);

        // Modify the shared data
        shared_data++;
        printf("Writer %zu modified shared_data to %d\n", (dthread_ullong_t)dthread_self(), shared_data);

        // Release write lock
        dthread_rwlock_unlock(&rwlock);

        // Simulate some work
        xsleep(2000);
    }
    return NULL;
}

int main()
{
    DThread readers[NUM_READERS];
    DThread writers[NUM_WRITERS];

    // Initialize the read-write lock
    dthread_rwlock_init(&rwlock);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++)
    {
        readers[i] = dthread_init_thread(reader, NULL);
        dthread_create(&readers[i], NULL);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        writers[i] = dthread_init_thread(writer, NULL);
        dthread_create(&writers[i], NULL);
    }

    // Join all threads
    for (int i = 0; i < NUM_READERS; i++)
    {
        dthread_join(&readers[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        dthread_join(&writers[i]);
    }

    dthread_rwlock_destroy(&rwlock);

    return 0;
}
