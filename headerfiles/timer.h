#ifndef TIMER_H
#define TIMER_H

#include "estructuras.h"

int spawnTimer(pthread_t *timer_thread, thread_args_t *args);
int timer_function(thread_args_t *args);

#endif
