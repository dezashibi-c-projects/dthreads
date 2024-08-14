// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: return.c
//    Date: 2024-08-14
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     -> THIS EXAMPLE IS ORIGINALLY FROM https://code-vault.net
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Refer to readme for documentation or dthread.h
// ***************************************************************************************

#define DTHREAD_DEBUG
#define DTHREAD_IMPL
#include "../dthreads/dthread.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

dthread_define_routine(roll_dice)
{
    (void)arg;

    srand(time(NULL));

    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed in roll_dice\n");
        dthread_exit(NULL);
    }

    *result = value;

    printf("Thread result: %p\n", (void*)result);
    return (void*)result;
}

int main(void)
{
    DThread thread;
    DThreadConfig th_conf;

    th_conf = dthread_config_init(roll_dice, NULL);

    if (dthread_create(&thread, NULL, &th_conf) != 0)
    {
        fprintf(stderr, "Thread create failed\n");
        return 1;
    }

    if (dthread_join(thread, &th_conf) != 0)
    {
        fprintf(stderr, "Thread join failed\n");
        return 2;
    }

    printf("Main res: %p\n", (void*)th_conf.result);
    printf("Result: %d\n", *(int*)(th_conf.result));

    free(th_conf.result);

    return 0;
}
