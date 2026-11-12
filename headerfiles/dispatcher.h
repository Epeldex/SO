#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "estructuras.h"

int spawnDispatcher(pthread_t *dispatcher_thread, dispatcher_args_t *args);
int dispatcher_function(dispatcher_args_t *args);
int execute_process(PCB_t **processQueue);

#endif
