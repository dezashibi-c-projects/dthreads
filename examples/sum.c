#define DTHREAD_IMPL
#include "../dthreads/dthread.h"

#include <stdio.h>
#include <stdlib.h>

unsigned long sum = 0;

dthread_define_routine(summation)
{
    unsigned long upper = *(unsigned long*)arg;
    for (unsigned long i = 0; i <= upper; ++i)
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

    unsigned long arg = atoi(argv[1]);
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