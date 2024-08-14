// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: trylock.c
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

DThreadMutex stove_mutex[4];
int stove_fuel[4] = {100, 100, 100, 100};

dthread_define_routine(routine)
{
    (void)data;
    srand(time(NULL));

    for (int i = 0; i < 4; i++)
    {
        if (dthread_mutex_trylock(&stove_mutex[i]) == 0)
        {
            int fuelNeeded = (rand() % 30);
            if (stove_fuel[i] - fuelNeeded < 0)
            {
                printf("No more fuel... going home\n");
            }
            else
            {
                stove_fuel[i] -= fuelNeeded;
                xsleep(1000);
                printf("Fuel left %d\n", stove_fuel[i]);
            }
            dthread_mutex_unlock(&stove_mutex[i]);
            break;
        }
        else
        {
            if (i == 3)
            {
                printf("No stove available yet, waiting...\n");
                xsleep(800);
                i = 0;
            }
        }
    }

    return NULL;
}

int main(void)
{
    DThread th[10];

    for (int i = 0; i < 4; i++)
    {
        dthread_mutex_init(&stove_mutex[i], NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        th[i] = dthread_new_config(routine, NULL);

        if (dthread_create(&th[i], NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (dthread_join(&th[i]) != 0)
        {
            perror("Failed to join thread");
        }
    }

    for (int i = 0; i < 4; i++)
    {
        dthread_mutex_destroy(&stove_mutex[i]);
    }

    return 0;
}
