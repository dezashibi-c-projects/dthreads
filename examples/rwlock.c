#define DTHREAD_DEBUG
#define DTHREAD_IMPL
#include "../dthreads/dthread.h"
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define xsleep(x) Sleep((x))
#else
void xsleep(uint32_t milliseconds)
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
        dthread_debug("Reader attempting to acquire read lock");

        // Acquire read lock
        dthread_rwlock_rdlock(&rwlock);

        dthread_debug("Reader acquired read lock");

        // Read the shared data
        printf("Reader %zu read shared_data = %d\n", dthread_self(), shared_data);

        // Release read lock
        dthread_rwlock_unlock(&rwlock);

        dthread_debug("Reader released read lock");

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
        dthread_debug("Writer attempting to acquire write lock");

        // Acquire write lock
        dthread_rwlock_wrlock(&rwlock);

        dthread_debug("Writer acquired write lock");

        // Modify the shared data
        shared_data++;
        printf("Writer %zu modified shared_data to %d\n", dthread_self(), shared_data);

        // Release write lock
        dthread_rwlock_unlock(&rwlock);

        dthread_debug("Writer released write lock");

        // Simulate some work
        xsleep(2000);
    }
    return NULL;
}

int main(void)
{
    DThread readers[NUM_READERS];
    DThread writers[NUM_WRITERS];

    // Initialize the read-write lock
    dthread_rwlock_init(&rwlock);
    dthread_debug("Read-write lock initialized");

    for (int i = 0; i < NUM_READERS; i++)
    {
        readers[i] = dthread_init_thread(reader, NULL);
        dthread_create(&readers[i], NULL);
        dthread_debug("Reader thread created");
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        writers[i] = dthread_init_thread(writer, NULL);
        dthread_create(&writers[i], NULL);
        dthread_debug("Writer thread created");
    }

    for (int i = 0; i < NUM_READERS; i++)
    {
        dthread_join(&readers[i]);
        dthread_debug("Reader thread joined");
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        dthread_join(&writers[i]);
        dthread_debug("Writer thread joined");
    }

    dthread_rwlock_destroy(&rwlock);
    dthread_debug("Read-write lock destroyed");

    return 0;
}
