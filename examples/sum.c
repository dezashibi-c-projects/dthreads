// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: sum.c
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

long sum = 0;

dthread_define_routine(summation)
{
    long upper = *(long*)arg;
    for (long i = 0; i <= upper; ++i)
    {
        ++sum;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    DThread thread;

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

    DThreadConfig conf = dthread_config_init(summation, &arg);

    if (dthread_create(&thread, NULL, &conf) != 0)
    {
        fprintf(stderr, "Creating thread failed\n");
        return 1;
    }

    if (dthread_join(thread, NULL) != 0)
    {
        fprintf(stderr, "Joining thread failed\n");
        return 1;
    }

    printf("sum = %lu\n", sum);

    return 0;
}
