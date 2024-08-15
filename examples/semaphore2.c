// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: semaphore2.c
//    Date: 2024-08-15
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     -> THIS EXAMPLE IS BASED ON A SIMILAR EXAMPLE FROM https://code-vault.net
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

#define THREAD_NUM 16

DThreadSemaphore semaphore;

dthread_define_routine(routine)
{
    dthread_rng_seed_maker();

    printf("(%d) Waiting in the login queue\n", *(int*)data);

    dthread_semaphore_wait(&semaphore);

    printf("(%d) Logged in\n", *(int*)data);
    xsleep((dthread_rng_random() % 5 + 1) * 1000);
    printf("(%d) Logged out\n", *(int*)data);

    dthread_semaphore_post(&semaphore);

    free(data);

    return NULL;
}

int main(void)
{
    dthread_rng_init();

    DThread th[THREAD_NUM];

    dthread_semaphore_init(&semaphore, 32);

    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        th[i] = dthread_new_config(routine, malloc(sizeof(int)));
        dthread_set_data(&th[i], int*, i);

        if (dthread_create(&th[i], NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (dthread_join(&th[i]) != 0)
        {
            perror("Failed to join thread");
        }
    }

    dthread_semaphore_destroy(&semaphore);
    dthread_rng_cleanup();

    return 0;
}
