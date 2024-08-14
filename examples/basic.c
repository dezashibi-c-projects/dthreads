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

long value = 0;

DThreadMutex mutex;

dthread_define_routine(thread_func)
{
    (void)arg;

    dthread_mutex_lock(&mutex);

    value += 1;
    printf("----- value is now %lu\n", value);

    dthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[])
{
    DThread threads[NUM_THREADS];
    DThreadConfig thread_configs[NUM_THREADS];

    if (argc != 2)
    {
        fprintf(stderr, "An integer parameter is required\n");
        return 1;
    }

    long arg = atoi(argv[1]);
    if (arg < 0)
    {
        fprintf(stderr, "An integer >= 0 is required\n");
        return 1;
    }

    value = arg;
    printf("----- value started at %lu\n", value);

    // Initialize mutex
    dthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        thread_configs[i] = dthread_config_init(thread_func, NULL);

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

    // Cleanup mutex
    dthread_mutex_destroy(&mutex);

    printf("----- value finished with %lu\n", value);

    return 0;
}
