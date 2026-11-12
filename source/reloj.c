#include "reloj.h"

#include "configuracion.h"

#include <unistd.h>

int clock_function(mutex_block_t *clock_mutex)
{
    int clock_counter = 0;
    double clock_interval = 1.0 / clockFreq;

    while (1)
    {
        if (clock_counter >= duration)
            break;
        pthread_mutex_lock(clock_mutex->mutex);
        sleep(clock_interval);
        clock_counter++;
        pthread_mutex_unlock(clock_mutex->mutex);
    }
    return 0;
}

int spawnClock(pthread_t *clock_thread, mutex_block_t *clock_mutex)
{
    return pthread_create(clock_thread, NULL, (void *)clock_function, clock_mutex);
}
