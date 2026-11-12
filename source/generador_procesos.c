#include "generador_procesos.h"

#include "cola_procesos.h"

#include <pthread.h>

int process_generator_function(pgen_args_t *args)
{
    while (1)
    {
        pthread_mutex_lock(args->pgen_mutex->mutex);
        addRandomNode(args->processQueue);
        pthread_mutex_unlock(args->pgen_mutex->mutex);
    }
    return 0;
}

int spawnProcessGenerator(pthread_t *pgen_thread, pgen_args_t *pgen_args)
{
    return pthread_create(pgen_thread, NULL, (void *)process_generator_function, pgen_args);
}
