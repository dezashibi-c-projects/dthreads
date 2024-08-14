#define _POSIX_C_SOURCE 200112L

#define DTHREAD_IMPL
#include "../dthreads/dthread.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define xsleep(x) Sleep((x))
#else
void xsleep(unsigned int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
#endif

#define NUM_THREADS 4

DThreadSemaphore sem;

dthread_define_routine(thread_func)
{
    int* thread_num = (int*)arg;

    dthread_semaphore_wait(&sem);

    xsleep(1);

    // Access the shared resource
    FILE* file = fopen("output.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "Thread %d wrote to the file.\n", *thread_num);
        fclose(file);
    }
    else
    {
        fprintf(stderr, "Failed to open file\n");
    }

    dthread_semaphore_post(&sem);

    return NULL;
}

int main(void)
{
    DThread threads[NUM_THREADS];
    DThreadConfig thread_configs[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    memset(thread_ids, 0, sizeof(int));

    // Initialize semaphore
    if (dthread_semaphore_init(&sem, 1) != 0)
    {
        fprintf(stderr, "Semaphore initialization failed\n");
        return EXIT_FAILURE;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        thread_ids[i] = i;
        thread_configs[i] = dthread_config_init(thread_func, &thread_ids[i]);

        if (dthread_create(&threads[i], NULL, &thread_configs[i]) != 0)
        {
            fprintf(stderr, "Failed to create thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        dthread_join(threads[i], NULL);
    }

    // Cleanup semaphore
    dthread_semaphore_destroy(&sem);

    printf("All threads have finished execution.\n");

    return EXIT_SUCCESS;
}
