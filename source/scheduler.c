#include "scheduler.h"

#include <pthread.h>

int spawnScheduler(pthread_t *scheduler_thread, scheduler_args_t *args)
{
    return pthread_create(scheduler_thread, NULL, (void *)scheduler_function, args);
}

int scheduler_function(scheduler_args_t *args)
{
    while (1)
    {
        pthread_mutex_lock(args->scheduler_mutex->mutex);
        // Aqui iria la logica del planificador
        pthread_mutex_unlock(args->scheduler_mutex->mutex);
    }
    return 0;
}
