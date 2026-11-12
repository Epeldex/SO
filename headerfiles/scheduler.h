#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "estructuras.h"

int spawnScheduler(pthread_t *scheduler_thread, scheduler_args_t *args);
int scheduler_function(scheduler_args_t *args);

#endif
