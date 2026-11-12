#include "maquina.h"

#include "configuracion.h"
#include "dispatcher.h"
#include "generador_procesos.h"
#include "reloj.h"
#include "scheduler.h"
#include "timer.h"

#include <stdio.h>

int spawnMachine(PCB_t **processQueue)
{
    pthread_t clock_thread, timer_thread, pgen_thread, scheduler_thread, dispatcher_thread;

    pthread_mutex_t timer_clock_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t pgen_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t scheduler_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t dispatcher_mutex = PTHREAD_MUTEX_INITIALIZER;

    mutex_block_t timer_mutex = {&timer_clock_mutex, 0};
    mutex_block_t pgen_mutex_block = {&pgen_mutex, 0};
    mutex_block_t scheduler_mutex_block = {&scheduler_mutex, 0};
    mutex_block_t dispatcher_mutex_block = {&dispatcher_mutex, 0};

    thread_args_t thread_args = {&timer_mutex, &pgen_mutex_block, &scheduler_mutex_block, &dispatcher_mutex_block};
    pgen_args_t pgen_args = {&pgen_mutex_block, processQueue};
    scheduler_args_t scheduler_args = {&scheduler_mutex_block, processQueue};
    dispatcher_args_t dispatcher_args = {&dispatcher_mutex_block, processQueue};

    printf("Spawning machine with %d CPUs, %d cores, and %d threads.\n", machine.ncpu, machine.ncore, machine.nthreads);

    if (spawnClock(&clock_thread, &timer_mutex) != 0)
    {
        fprintf(stderr, "Error creating clock thread\n");
        return 1;
    }

    if (spawnTimer(&timer_thread, &thread_args) != 0)
    {
        fprintf(stderr, "Error creating timer thread\n");
        return 1;
    }

    if (spawnProcessGenerator(&pgen_thread, &pgen_args) != 0)
    {
        fprintf(stderr, "Error creating process generator thread\n");
        return 1;
    }

    if (spawnScheduler(&scheduler_thread, &scheduler_args) != 0)
    {
        fprintf(stderr, "Error creating scheduler thread\n");
        return 1;
    }
    if (spawnDispatcher(&dispatcher_thread, &dispatcher_args) != 0)
    {
        fprintf(stderr, "Error creating dispatcher thread\n");
        return 1;
    }

    pthread_join(clock_thread, NULL);
    pthread_join(timer_thread, NULL);
    pthread_join(pgen_thread, NULL);
    pthread_join(scheduler_thread, NULL);
    pthread_join(dispatcher_thread, NULL);

    return 0;
}
