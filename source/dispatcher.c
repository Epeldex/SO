#include "dispatcher.h"

#include "cola_procesos.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int spawnDispatcher(pthread_t *dispatcher_thread, dispatcher_args_t *args)
{
    return pthread_create(dispatcher_thread, NULL, (void *)dispatcher_function, args);
}

int dispatcher_function(dispatcher_args_t *args)
{
    while (1)
    {
        pthread_mutex_lock(args->dispatcher_mutex->mutex);
        execute_process(args->processQueue);
        pthread_mutex_unlock(args->dispatcher_mutex->mutex);
    }
    return 0;
}

int execute_process(PCB_t **processQueue)
{
    PCB_t *node = dequeue(processQueue);
    if (node != NULL)
    {
        printf("Executing process with PID: %d and lifetime: %lf\n", node->pid, node->lifetime);
        free(node);
    }
    else
    {
        printf("No process to execute.\n");
    }
    return 0;
}
