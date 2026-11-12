#include "timer.h"

#include "configuracion.h"

int spawnTimer(pthread_t *timer_thread, thread_args_t *args)
{
    return pthread_create(timer_thread, NULL, (void *)timer_function, args);
}

int timer_function(thread_args_t *args)
{
    int clock = 0;
    while (1)
    {
        pthread_mutex_lock(args->timer_mutex->mutex);
        clock++;
        if (args->pgen_mutex->flag == 0)
        {
            pthread_mutex_lock(args->pgen_mutex->mutex);
            args->pgen_mutex->flag = 1;
        }
        else if (args->scheduler_mutex->flag == 0)
        {
            pthread_mutex_lock(args->scheduler_mutex->mutex);
            args->scheduler_mutex->flag = 1;
        }
        else if (args->dispatcher_mutex->flag == 0)
        {
            pthread_mutex_lock(args->dispatcher_mutex->mutex);
            args->dispatcher_mutex->flag = 1;
        }

        if (clock % ticks_per_clock == 0)
        {
            if (args->pgen_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->pgen_mutex->mutex);
                args->pgen_mutex->flag = 0;
            }
            else if (args->scheduler_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->scheduler_mutex->mutex);
                args->scheduler_mutex->flag = 0;
            }
            else if (args->dispatcher_mutex->flag == 1)
            {
                pthread_mutex_unlock(args->dispatcher_mutex->mutex);
                args->dispatcher_mutex->flag = 0;
            }
        }
        pthread_mutex_unlock(args->timer_mutex->mutex);
    }
    return 0;
}
