// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: basic.c
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

#define DTHREAD_IMPL
#define DTHREAD_DEBUG
#include "../dthreads/dthread.h"

#include <stdio.h>

#define NUM_THREADS 4

long value = 12000;

DThreadMutex mutex;

dthread_define_routine(thread_func)
{
    (void)data;

    dthread_mutex_lock(&mutex);

    value += 1;
    dthread_debug_args("----- value is now %lu", value);

    dthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    DThread threads[NUM_THREADS];

    dthread_debug_args("----- value started at %lu", value);

    // Initialize mutex
    dthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = dthread_new_config(thread_func, NULL);

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

    // Cleanup mutex
    dthread_mutex_destroy(&mutex);

    dthread_debug_args("----- value finished with %lu", value);

    printf("final result: %lu\n", value);

    return 0;
}
