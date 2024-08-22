// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: semaphore.c
//    Date: 2024-08-14
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Refer to readme for documentation or dthread.h
// ***************************************************************************************

#define DTHREAD_DEBUG
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
void xsleep(dthread_uint_t milliseconds)
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
    int* thread_num = (int*)data;

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
    int thread_ids[NUM_THREADS];

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
        threads[i] = dthread_init_thread(thread_func, &thread_ids[i]);

        if (dthread_create(&threads[i], NULL) != 0)
        {
            fprintf(stderr, "Failed to create thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        dthread_join(&threads[i]);
    }

    // Cleanup semaphore
    dthread_semaphore_destroy(&sem);

    printf("All threads have finished execution.\n");

    return EXIT_SUCCESS;
}
