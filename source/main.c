#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "configuracion.h"
#include "cola_procesos.h"
#include "maquina.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t machine_creation_thread;

    setParameters(argv, argc);
    printMachineParameters();
    PCB_t *processQueue = create_processQueue();

    if (pthread_create(&machine_creation_thread, NULL, (void *)spawnMachine, &processQueue) != 0)
    {
        fprintf(stderr, "Error creating machine creation thread\n");
        free_processQueue(processQueue);
        return 1;
    }

    free_processQueue(processQueue);
    pthread_join(machine_creation_thread, NULL);
    return 0;
}
