// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: sum.c
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

#define DTHREAD_IMPL
#include "../dthreads/dthread.h"

#include <stdio.h>
#include <stdlib.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

dthread_define_routine(routine)
{
    int index = *(int*)data;
    int sum = 0;

    for (int j = 0; j < 5; ++j)
    {
        sum += primes[index + j];
    }

    printf("Local sum: %d\n", sum);
    *(int*)data = sum;
    return data;
}

int main(void)
{
    DThread th[2];

    int i;

    for (i = 0; i < 2; ++i)
    {
        th[i] = dthread_new_config(routine, malloc(sizeof(int)));

        dthread_set_data(&th[i], int*, i * 5);

        if (dthread_create(&th[i], NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }

    int globalSum = 0;

    for (i = 0; i < 2; ++i)
    {
        if (dthread_join(&th[i]) != 0)
        {
            perror("Failed to join thread");
        }

        globalSum += *dthread_get_result_as(&th[i], int*);

        free(dthread_get_result(&th[i]));
    }

    printf("Global sum: %d\n", globalSum);
    return 0;
}
