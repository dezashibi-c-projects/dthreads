// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: barrier.c
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
#include "../dthreads/dthread.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4

DThreadBarrier barrier;

dthread_define_routine(thread_func)
{
    int thread_num = *((int*)data);

    printf("Thread %d reached the barrier.\n", thread_num);

    dthread_barrier_wait(&barrier);

    printf("Thread %d passed the barrier.\n", thread_num);

    return NULL;
}

int main(void)
{
    DThread threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize barrier
    dthread_barrier_init(&barrier, NUM_THREADS);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        thread_ids[i] = i;
        threads[i] = dthread_new_config(thread_func, &thread_ids[i]);
        dthread_create(&threads[i], NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        dthread_join(&threads[i]);
    }

    // Cleanup barrier
    dthread_barrier_destroy(&barrier);

    return 0;
}
